//
// Created by xinnjie on 2019/9/10.
//

#include "command_dispatcher.h"
#include <spdlog/spdlog.h>
namespace littleB {

void CommandDistributor::read(Context *ctx, ServiceTuple msg) {
    uint32_t cmd_id = std::get<0>(msg);
    std::shared_ptr<RoleInfo> role_ptr = std::get<1>(msg);
    MessagePtr request_ptr = std::move(std::get<2>(msg));
    assert(command_manager_.IsValidCmd(cmd_id));
    if (command_manager_.IsSyncCmd(cmd_id)) {
        SPDLOG_TRACE("[CommandDistributor] RunSyncService CMD={}", cmd_id);
        MessagePtr response_ptr = command_manager_.RunSyncService(cmd_id, *role_ptr, *request_ptr);
        write(ctx, std::make_pair(cmd_id, std::move(response_ptr)));
    } else if (command_manager_.IsAsyncCmd(cmd_id)) {
        SPDLOG_TRACE("[CommandDistributor] RunAsyncService CMD={}", cmd_id);
        folly::Future<MessagePtr> rsp_future = command_manager_.RunAsyncService(cmd_id, *role_ptr, *request_ptr);
        std::move(rsp_future).thenValue([this, ctx, cmd_id](MessagePtr response_ptr) {
            this->write(ctx, std::make_pair(cmd_id, std::move(response_ptr)));
        });

    } else {
        SPDLOG_WARN("[CommandDistributor] INVALID CMD={}", cmd_id);
        return;
    }
}

}