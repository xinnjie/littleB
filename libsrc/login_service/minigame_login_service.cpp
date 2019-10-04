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
        // TODO 补全 login 逻辑
        if (!PullRoleInfoFromDB(username)) {
            SPDLOG_WARN("pull role failed when having correct password, something is wrong");
            ret = LoginRsp::UNKOWN_EORROR;
            break;
        }
    } while (0);
    rsp.set_error(ret);
    rsp.set_message(LoginRsp::ErrorCode_Name(static_cast<LoginRsp::ErrorCode>(ret)));
    return rsp;
}
std::shared_ptr<RoleInfo> MinigameFakeLoginService::PullRoleInfoFromDB(const std::string& username) {
    auto reply = redis_wrapper_.RedisCommand("get __role_%s", username.c_str());
    if (reply->type != REDIS_REPLY_STRING) {
        SPDLOG_WARN("no role in DB, something wrong happened during registration");
        return std::shared_ptr<RoleInfo>(nullptr);
    }
    auto role = std::make_shared<RoleInfo>();
    if (!role->ParseFromArray(reply->str, reply->len)) {
        SPDLOG_WARN("decode error");
        return std::shared_ptr<RoleInfo>(nullptr);
    }
    return role;
}
