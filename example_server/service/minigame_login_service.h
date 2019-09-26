//
// Created by xinnjie on 2019/9/26.
//

#ifndef LITTLEB_MINIGAME_LOGIN_SERVICE_H
#define LITTLEB_MINIGAME_LOGIN_SERVICE_H
#include "service_interface.h"
#include "sync_redis_wrapper.h"
#include "data_manager/roleinfo_manager.h"
#include "login.pb.h"
class MinigameLoginService : public littleB::SyncServiceInterface<C2R_Login, R2C_Login>{
public:
    MinigameLoginService(littleB::SyncRedisWrapper& redisWrapper, littleB::RoleinfoManager& roleManager)
        : redis_wrapper_(redisWrapper), role_manager_(roleManager) {}
    R2C_Login operator()(RoleInfo& role, const C2R_Login& request) override;

    bool PullRoleInfoFromDB(const folly::SocketAddress& remote_address, const std::string& username);

private:
    littleB::SyncRedisWrapper &redis_wrapper_;
    littleB::RoleinfoManager &role_manager_;
};

#endif  // LITTLEB_MINIGAME_LOGIN_SERVICE_H
