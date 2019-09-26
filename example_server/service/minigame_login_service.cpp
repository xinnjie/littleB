//
// Created by xinnjie on 2019/9/26.
//

#include "minigame_login_service.h"
#include <spdlog/spdlog.h>

R2C_Login MinigameLoginService::operator()(RoleInfo& role, const C2R_Login& request) {
    assert(!role.has_basic_info() && !role.has_progress());
    R2C_Login rsp;
    rsp.set_rpcid(request.rpcid());

    const std::string& username = request.account();
    const std::string& password = request.password();
    auto reply = redis_wrapper_.RedisCommand("get __password-%s", username.c_str());
    if (reply->type == REDIS_REPLY_NIL) {
        SPDLOG_INFO("user login: no role|username={}|password={}", request.account(), request.password());
        rsp.set_error(-1);
        return rsp;
    }
    SPDLOG_INFO("user login|username={}|password={}|actual_password={}", request.account(), request.password(), reply->str);
    // TODO 补全 login 逻辑
    rsp.set_error(0);
    rsp.set_message("login success");
    return rsp;
}
