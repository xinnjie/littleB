//
// Created by xinnjie on 2019/9/25.
//

#ifndef LITTLEB_ROLEINFO_MANAGER_H
#define LITTLEB_ROLEINFO_MANAGER_H
#include <folly/SocketAddress.h>
#include <map>
#include "role_info.h"
namespace littleB {
/**
 * 使用远端 ip 标识玩家
 */
class RoleinfoManager {
public:
    bool AddRole(const folly::SocketAddress& remote_address, std::shared_ptr<RoleInfo> role_info) {
        return roles_.insert(std::make_pair(remote_address, role_info)).second;
    }
    bool RemoveRole(const folly::SocketAddress& remote_address) {
        return roles_.erase(remote_address) > 0;
    }
    std::shared_ptr<RoleInfo> GetRole(const folly::SocketAddress& remote_address);

private:
    std::map<folly::SocketAddress, std::shared_ptr<RoleInfo>> roles_;
};
}  // namespace littleB
#endif  // LITTLEB_ROLEINFO_MANAGER_H
