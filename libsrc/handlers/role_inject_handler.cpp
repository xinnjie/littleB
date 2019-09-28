//
// Created by xinnjie on 2019/9/25.
//
#include "role_inject_handler.h"
#include <spdlog/spdlog.h>
#include <boost/pointer_cast.hpp>
#include "cmd_id.pb.h"
#include "login.pb.h"
#include "login_service/minigame_login_service.h"
namespace littleB {
void RoleInjectHandler::read(Context *ctx, CmdMessagePair msg) {
    auto addr = ctx->getTransport()->getPeerAddress();
    uint32_t cmd_id = msg.first;
    if (cmd_id == LOGIN) {
        static RoleInfo empty_role;
        MinigameFakeLoginService login_service(redis_wrapper_);
        std::unique_ptr<LoginReq> req = boost::dynamic_pointer_cast<LoginReq>(std::move(msg.second));
        auto role_ptr = login_service.PullRoleInfoFromDB(req->account());
        if (role_ptr) {
            role_manager_.AddRole(addr, role_ptr);
            SPDLOG_INFO("username={}|address={} login and has pulled role data", req->account(), addr.describe());
        }
        std::unique_ptr<LoginRsp> rsp = std::make_unique<LoginRsp>();
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
    SPDLOG_INFO("address={} login and has pulled role data", addr.describe());
    ctx->fireRead(std::make_tuple(msg.first, role, std::move(msg.second)));
}
void RoleInjectHandler::transportInactive(Context *ctx) { Handler::transportInactive(ctx); }
}