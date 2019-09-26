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
#include "service/minigame_login_service.h"
#include "sync_redis_wrapper.h"

using namespace folly;
using namespace wangle;
using namespace littleB;

using LittlebPipeline = Pipeline<IOBufQueue &, ServiceTuple>;

class LittleBPipelineFactory : public PipelineFactory<LittlebPipeline> {
public:
    LittleBPipelineFactory(RoleinfoManager &roleManager, CommandManager &commandManager,
                           PbReflectionManager &reflectionManager)
        : role_manager_(roleManager), command_manager_(commandManager), reflection_manager_(reflectionManager) {}
    LittlebPipeline::Ptr newPipeline(std::shared_ptr<AsyncTransportWrapper> sock) override {
        auto pipeline = LittlebPipeline::create();
        pipeline->addBack(AsyncSocketHandler(sock));
        pipeline->addBack(LengthFieldBasedFrameDecoder(4, 65536, 0, 0, 2, true));
        pipeline->addBack(LengthFieldPrepender(4, 0, false, true));
        pipeline->addBack(OpcodeInjectHandler());
        pipeline->addBack(CmdMessageSerializeHandler(reflection_manager_));
        pipeline->addBack(RoleInjectHandler(role_manager_));
        pipeline->addBack(CommandDistributor(command_manager_));
        pipeline->finalize();
        return pipeline;
    }

private:
    RoleinfoManager &role_manager_;
    CommandManager &command_manager_;
    PbReflectionManager &reflection_manager_;
};

int main(int argc, char **argv) {
    ServerBootstrap<LittlebPipeline> server;
    RoleinfoManager role_manager;
    CommandManager command_manager;
    PbReflectionManager reflection_manager;
    SyncRedisWrapper redis_wrapper;
    redis_wrapper.Connect("127.0.0.1", 6379, timeval{ 1, 500000 });
    RegisterSyncCommand<MinigameLoginService>(command_manager, reflection_manager, 31, redis_wrapper);
    server.childPipeline(std::make_shared<LittleBPipelineFactory>(role_manager, command_manager, reflection_manager));
    server.bind(8009);
    server.waitForStop();

    return 0;
}