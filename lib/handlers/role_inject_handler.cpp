//
// Created by xinnjie on 2019/9/25.
//
#include "role_inject_handler.h"
#include <spdlog/spdlog.h>

void littleB::RoleInjectHandler::read(Context *ctx, littleB::CmdMessagePair msg) {
    auto addr = ctx->getTransport()->getPeerAddress();
    std::shared_ptr<RoleInfo> role = role_manager_.GetRole(addr);
    if (!role) {
        SPDLOG_INFO("address={} not login or has some problem", addr.describe());
        role = std::make_shared<RoleInfo>();
    }
    ctx->fireRead(std::make_tuple(msg.first, role, std::move(msg.second)));
}
