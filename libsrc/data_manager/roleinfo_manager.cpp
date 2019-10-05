//
// Created by xinnjie on 2019/9/25.
//

#include "roleinfo_manager.h"
std::shared_ptr<RoleInfo> littleB::RoleinfoManager::GetRole(const folly::SocketAddress& remote_address) {
    auto iter = roles_.find(remote_address);
    if (iter != roles_.cend()) {
        return iter->second;
    }
    return std::shared_ptr<RoleInfo>(nullptr);
}

std::shared_ptr<RoleInfo> littleB::RoleinfoManager::PullRoleInfoFromDB(const std::string &username) {
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

bool littleB::RoleinfoManager::PushRoleToDB(const folly::SocketAddress &remote_address) {
    auto iter = roles_.find(remote_address);
    if (iter == roles_.cend()) {
        return false;
    }
    return PushRoleToDB(iter->second);
}

bool littleB::RoleinfoManager::PushRoleToDB(std::shared_ptr<RoleInfo> role) {
    if (!role) {
        return false;
    }
    return PushRoleToDB(*role);
}

bool littleB::RoleinfoManager::PushRoleToDB(const RoleInfo &role) {
    std::string buf;
    role.SerializeToString(&buf);
    auto reply = redis_wrapper_.RedisCommand("set __role_%s %b", role.basic_info().username().c_str(), buf.data(), buf.size());
    if (reply->type != REDIS_REPLY_STATUS) {
        SPDLOG_WARN("push role_info failed | username={}", role.basic_info().username());
        return false;
    }
    SPDLOG_INFO("push to DB succeed | role_info={}", role.ShortDebugString());
    return true;
}
