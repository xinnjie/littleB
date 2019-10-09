//
// Created by xinnjie on 2019/9/24.
//
// Set global log level to trace
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <wangle/bootstrap/ServerBootstrap.h>
#include <wangle/channel/AsyncSocketHandler.h>
#include <wangle/channel/EventBaseHandler.h>

#include "cmd_id.pb.h"
#include "common_def.h"
#include "data_manager/command_manager.h"
#include "data_manager/pb_reflection_manager.h"
#include "data_manager/roleinfo_manager.h"
#include "data_manager/task_data_manager.h"
#include "handlers/LengthFieldBasedFrameDecoder.h"
#include "handlers/LengthFieldPrepender.h"
#include "handlers/cmd_message_serialize_handler.h"
#include "handlers/command_dispatcher.h"
#include "handlers/opcode_inject_handler.h"
#include "handlers/role_inject_handler.h"
#include "login_service/minigame_login_service.h"
#include "login_service/minigame_register_service.h"
#include "service/task_query_service.h"
#include "service/task_update_service.h"
#include "service/erase_role_service.h"
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
        /* minigame 客户端发包，包长度字段为 2 字节，非网络字节序，长度不包含包长度字段 */
        pipeline->addBack(
                littleB::LengthFieldBasedFrameDecoder(PKG_LENGTH_FIELD_SIZE, 65536, 0, 0, PKG_LENGTH_FIELD_SIZE,
                                                      false));
        pipeline->addBack(littleB::LengthFieldPrepender(PKG_LENGTH_FIELD_SIZE, 0, false, false));
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

void prototest() {
    LoginReq req;
    req.set_account("");
    req.set_password("111111");
    req.set_rpcid(LOGIN);
    SPDLOG_INFO("proto size={}", req.ByteSize());
}

void prepareAndCheck(SyncRedisWrapper &redis_wrapper) {
    assert(redis_wrapper.RedisCommand("set __password_%s %s", "hello", "world")->type == REDIS_REPLY_STATUS);
    RoleInfo role;
    auto basic_ptr = role.mutable_basic_info();
    basic_ptr->set_player_id(31);
    basic_ptr->set_username("hello");
    auto progress_ptr = role.mutable_progress();
    progress_ptr->set_main_task_id(99);
    std::string buf;
    assert(role.SerializeToString(&buf));
    assert(redis_wrapper.RedisCommand("set __role_%s %b", "hello", buf.data(), buf.size()));
    auto reply = redis_wrapper.RedisCommand("get __role_%s", "hello");
    assert(reply->type == REDIS_REPLY_STRING);
    RoleInfo query_role;
    query_role.ParseFromArray(reply->str, reply->len);
    assert(query_role.basic_info().player_id() == query_role.basic_info().player_id());
    assert(query_role.progress().main_task_id() == query_role.progress().main_task_id());
}

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::trace);  // Set global log level to trace
    spdlog::set_pattern("[%H:%M:%S %z] [%l] [thread %t] [%s:%#] [%!] %v");

    ServerBootstrap<LittlebPipeline> server;
    CommandManager command_manager;
    PbReflectionManager reflection_manager;
    SyncRedisWrapper redis_wrapper;
    RoleinfoManager role_manager(redis_wrapper);
    TaskDataManager task_manager(redis_wrapper);


    redis_wrapper.Connect("127.0.0.1", 6379, timeval{1, 500000});

    prepareAndCheck(redis_wrapper);

    prototest();

    /* register services */
    RegisterSyncCommand<MinigameFakeLoginService>(command_manager, reflection_manager, LOGIN, redis_wrapper,
                                                  role_manager);
    RegisterSyncCommand<MinigameRegisterService>(command_manager, reflection_manager, REGISTER, redis_wrapper);
    RegisterSyncCommand<TaskQueryService>(command_manager, reflection_manager, QUERY_TASK, task_manager);
    RegisterSyncCommand<TaskUpdateService>(command_manager, reflection_manager, UPDATE_TASK, task_manager);
    RegisterSyncCommand<EraseRoleService>(command_manager, reflection_manager, ERASE_ROLE, role_manager);
    MinigameRegisterService register_service(redis_wrapper);
    RegisterReq req;
    req.set_rpcid(10);
    req.set_password("2");
    req.set_account("2");
    RoleInfo role;
    auto rsp = register_service(role, req);
    //    RegisterSyncCommand<MinigameLoginService>(command_manager, reflection_manager, 31, redis_wrapper);
    server.childPipeline(
            std::make_shared<LittleBPipelineFactory>(role_manager, command_manager, reflection_manager, redis_wrapper));
    server.bind(10002);
    server.waitForStop();

    return 0;
}
