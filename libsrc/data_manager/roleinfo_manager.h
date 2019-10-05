//
// Created by xinnjie on 2019/9/25.
//

#ifndef LITTLEB_ROLEINFO_MANAGER_H
#define LITTLEB_ROLEINFO_MANAGER_H
#include <map>
#include <mutex>
#include <folly/SocketAddress.h>
#include "sync_redis_wrapper.h"
#include "role.pb.h"
namespace littleB {
/**
 * 使用远端 ip 标识玩家
 */
class RoleinfoManager {
public:
    explicit RoleinfoManager(SyncRedisWrapper &redisWrapper) : redis_wrapper_(redisWrapper) {}

    /* 以下方法并非提供给 Service */
    bool AddRole(const folly::SocketAddress &remote_address, std::shared_ptr<RoleInfo> role_info) {
        std::lock_guard<std::mutex> guard(roles_mutex_);
        return roles_.insert(std::make_pair(remote_address, role_info)).second;
    }

    bool RemoveRole(const folly::SocketAddress &remote_address) {
        std::lock_guard<std::mutex> guard(roles_mutex_);
        return roles_.erase(remote_address) > 0;
    }

    std::shared_ptr<RoleInfo> GetRole(const folly::SocketAddress &remote_address);

    /**
     * 由于 Service 对网络是不知情的，因此不知道 remote_address，所以一般不用这个接口
     * @param remote_address
     * @return
     */
    bool PushRoleToDB(const folly::SocketAddress& remote_address);

    std::shared_ptr<RoleInfo> PullRoleInfoFromDB(const std::string &username);

    bool PushRoleToDB(std::shared_ptr<RoleInfo> role);

    /* 以下方法 Service 可以调用 */
    bool PushRoleToDB(const RoleInfo &role);


private:
    std::map<folly::SocketAddress, std::shared_ptr<RoleInfo>> roles_;
    // todo: 针对连接 consistent hash到针对单线程的roleinfo_manager。这样线程间不会有冲突
    std::mutex roles_mutex_;
    SyncRedisWrapper& redis_wrapper_;
};
}  // namespace littleB
#endif  // LITTLEB_ROLEINFO_MANAGER_H
