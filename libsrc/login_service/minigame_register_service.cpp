//
// Created by xinnjie on 2019/9/28.
//

#include "minigame_register_service.h"

RegisterRsp MinigameRegisterService::operator()(RoleInfo& role, const RegisterReq& request) {
    assert(!role.has_basic_info() && !role.has_progress());
    RegisterRsp rsp;
    rsp.set_rpcid(request.rpcid());
    RegisterRsp::ErrorCode ret = RegisterRsp::SUCCEED;

    const std::string& username = request.account();
    const std::string& password = request.password();
    do {
        auto reply = redis_wrapper_.RedisCommand("get __password_%s", username.c_str());
        if (reply->type != REDIS_REPLY_NIL) {
            ret = RegisterRsp::ACCOUNT_ALREADY_EXIST;
            break;
        }
        reply = redis_wrapper_.RedisCommand("set __password_%s %s", username.c_str(), password.c_str());
        if (reply->type != REDIS_REPLY_STATUS) {
            SPDLOG_WARN("DB set password faild | error_code={}", reply->type);
            ret = RegisterRsp::UNKOWN_EORROR;
            break;
        }
        // get gid
        reply = redis_wrapper_.RedisCommand("incr gid");
        if (reply->type != REDIS_REPLY_INTEGER) {
            SPDLOG_WARN("DB incr password faild | error_code={}", reply->type);
            ret = RegisterRsp::UNKOWN_EORROR;
            break;
        }
        int gid = reply->integer;
        RoleInfo initial_role;
        initial_role.mutable_basic_info()->set_player_id(gid);
        std::string role_buf;
        initial_role.SerializeToString(&role_buf);
        reply = redis_wrapper_.RedisCommand("set __role_%s %b", username.c_str(), role_buf.data(), role_buf.size());
        if (reply->type == REDIS_REPLY_STATUS) {
            SPDLOG_INFO("register succeed | username={} | gid={} | roleinfo={}", username, gid, initial_role.ShortDebugString());
        } else {
            SPDLOG_WARN("register failed | username={}", username);
            ret = RegisterRsp::UNKOWN_EORROR;
        }
    } while (0);
    rsp.set_error(ret);
    rsp.set_message(RegisterRsp::ErrorCode_Name(ret));
    return rsp;
}
