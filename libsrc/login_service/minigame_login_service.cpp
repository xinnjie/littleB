//
// Created by xinnjie on 2019/9/26.
//

#include "minigame_login_service.h"
#include <spdlog/spdlog.h>

LoginRsp MinigameFakeLoginService::operator()(RoleInfo&, const LoginReq& request) {
    LoginRsp rsp;
    rsp.set_rpcid(request.rpcid());
    int ret = LoginRsp::SUCCEED;
    do {
        const std::string& username = request.account();
        const std::string& password = request.password();
        auto reply = redis_wrapper_.RedisCommand("get __password_%s", username.c_str());
        if (reply->type != REDIS_REPLY_STRING) {
            SPDLOG_INFO("user login: not registered | username={} | password={}", request.account(),
                        request.password());
            ret = LoginRsp::LOGIN_NOT_REGISTERED;
            break;
        }
        SPDLOG_INFO("user login| username={} | password={} | actual_password={}", request.account(), request.password(),
                    reply->str);
        if (request.password() != reply->str) {
            SPDLOG_WARN("incorrect password | username={} | password={} | actual_password={}", request.account(), request.password(),
                        reply->str);
            ret = LoginRsp::LOGIN_PASSWORD_WRONG;
            break;
        }
        auto role_ptr = role_manager_.PullRoleInfoFromDB(username);
        if (!role_ptr) {
            SPDLOG_WARN("pull role failed when having correct password, something is wrong");
            ret = LoginRsp::UNKOWN_EORROR;
            break;
        }
        if (!role_ptr->basic_info().is_returning_visitor()) {
            role_ptr->mutable_basic_info()->set_is_returning_visitor(true);
            role_manager_.PushRoleToDB(role_ptr);
            rsp.set_is_returning_visitor(false);
        } else {
            rsp.set_is_returning_visitor(true);
        }

    } while (0);
    rsp.set_error(ret);
    rsp.set_message(LoginRsp::ErrorCode_Name(static_cast<LoginRsp::ErrorCode>(ret)));
    return rsp;
}
