//
// Created by xinnjie on 2019/9/24.
//
#include <wangle/bootstrap/ServerBootstrap.h>
#include <wangle/channel/AsyncSocketHandler.h>
#include <wangle/codec/LengthFieldBasedFrameDecoder.h>
#include <wangle/codec/LengthFieldPrepender.h>

#include "common_def.h"
#include "data_manager/command_manager.h"
#include "data_manager/pb_reflection_manager.h"
#include "data_manager/roleinfo_manager.h"
#include "handlers/cmd_message_serialize_handler.h"
#include "handlers/command_dispatcher.h"
#include "handlers/opcode_inject_handler.h"
#include "handlers/role_inject_handler.h"
#include "register_helper.h"
#include "sync_redis_wrapper.h"

using namespace folly;
using namespace wangle;
using namespace littleB;

using LittlebPipeline = Pipeline<IOBufQueue &, ServiceTuple>;

class LittleBPipelineFactory : public PipelineFactory<LittlebPipeline> {
public:
    LittleBPipelineFactory(RoleinfoManager &roleManager, CommandManager &commandManager,
                           PbReflectionManager &reflectionManager, SyncRedisWrapper &redisWrapper)
        : role_manager_(roleManager),
          command_manager_(commandManager),
          reflection_manager_(reflectionManager),
          redis_wrapper(redisWrapper) {}
    LittlebPipeline::Ptr newPipeline(std::shared_ptr<AsyncTransportWrapper> sock) override {
        auto pipeline = LittlebPipeline::create();
        pipeline->addBack(AsyncSocketHandler(sock));
        pipeline->addBack(LengthFieldBasedFrameDecoder(PKG_LENGTH_FIELD_SIZE, 65536, 0, 0, PKG_LENGTH_FIELD_SIZE, true));
        pipeline->addBack(LengthFieldPrepender(PKG_LENGTH_FIELD_SIZE, 0, false, true));
        pipeline->addBack(OpcodeInjectHandler());
        pipeline->addBack(CmdMessageSerializeHandler(reflection_manager_));
        pipeline->addBack(RoleInjectHandler(role_manager_, redis_wrapper));
        pipeline->addBack(CommandDistributor(command_manager_));
        pipeline->finalize();
        return pipeline;
    }

private:
    RoleinfoManager &role_manager_;
    CommandManager &command_manager_;
    PbReflectionManager &reflection_manager_;
    SyncRedisWrapper &redis_wrapper;
};
void prepareAndCheck(SyncRedisWrapper &redis_wrapper) {
    assert(redis_wrapper.RedisCommand("set __password_%s %s", "hello", "world")->type == REDIS_REPLY_STATUS);
    RoleInfo role;
    auto basic_ptr = role.mutable_basic_info();
    basic_ptr->set_player_id("1234567");
    auto progress_ptr = role.mutable_progress();
    progress_ptr->set_main_task_id(99);
    std::string buf;
    assert(role.SerializeToString(&buf));
    assert(redis_wrapper.RedisCommand("set __role_%s %b", "hello", buf.c_str(), role.ByteSizeLong()));
    auto reply = redis_wrapper.RedisCommand("get __role_%s", "hello");
    assert(reply->type == REDIS_REPLY_STRING);
    RoleInfo query_role;
    query_role.ParseFromArray(reply->str, reply->len);
    assert(query_role.basic_info().player_id() == query_role.basic_info().player_id());
    assert(query_role.progress().main_task_id() == query_role.progress().main_task_id());
}
int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::trace); // Set global log level to trace


    ServerBootstrap<LittlebPipeline> server;
    RoleinfoManager role_manager;
    CommandManager command_manager;
    PbReflectionManager reflection_manager;
    SyncRedisWrapper redis_wrapper;
    redis_wrapper.Connect("127.0.0.1", 6379, timeval{1, 500000});

    prepareAndCheck(redis_wrapper);

    //    RegisterSyncCommand<MinigameLoginService>(command_manager, reflection_manager, 31, redis_wrapper);
    server.childPipeline(
        std::make_shared<LittleBPipelineFactory>(role_manager, command_manager, reflection_manager, redis_wrapper));
    server.bind(10002);
    server.waitForStop();

    return 0;
}