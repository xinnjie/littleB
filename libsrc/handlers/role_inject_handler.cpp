//
// Created by xinnjie on 2019/9/25.
//
#include "role_inject_handler.h"
#include <spdlog/spdlog.h>
#include <boost/pointer_cast.hpp>
#include "login_service/minigame_login_service.h"
#include "cmd_id.pb.h"
#include "login.pb.h"

void littleB::RoleInjectHandler::read(Context *ctx, littleB::CmdMessagePair msg) {
    auto addr = ctx->getTransport()->getPeerAddress();
    uint32_t cmd_id = msg.first;
    if (cmd_id == LOGIN) {
        static RoleInfo empty_role;
        MinigameFakeLoginService login_service(redis_wrapper_, role_manager_);
        std::unique_ptr<C2R_Login> req  = boost::dynamic_pointer_cast<C2R_Login>(std::move(msg.second));
        auto role_ptr = login_service.PullRoleInfoFromDB(req->account());
        if (role_ptr) {
            role_manager_.AddRole(addr, role_ptr);
        }
        std::unique_ptr<R2C_Login> rsp = std::make_unique<R2C_Login>();
        *rsp = login_service(empty_role, *req);
        ctx->fireWrite(std::make_pair(cmd_id, std::move(rsp)));
        return;
    }
    /* 正常逻辑就是查到 role, 然后传递给后一个 handler */
    std::shared_ptr<RoleInfo> role = role_manager_.GetRole(addr);
    if (!role) {
        SPDLOG_INFO("address={} not login or has some problem", addr.describe());
        role = std::make_shared<RoleInfo>();
    }
    ctx->fireRead(std::make_tuple(msg.first, role, std::move(msg.second)));
}
