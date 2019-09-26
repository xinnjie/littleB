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
