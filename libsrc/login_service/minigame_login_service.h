//
// Created by xinnjie on 2019/9/26.
//

#ifndef LITTLEB_MINIGAME_LOGIN_SERVICE_H
#define LITTLEB_MINIGAME_LOGIN_SERVICE_H
#include "service_interface.h"
#include "sync_redis_wrapper.h"
#include "data_manager/roleinfo_manager.h"
#include "login.pb.h"
/**
 * Fake: 由外部调用 PullRoleInfoFromDB 获取Role, 本身的 operator() 只负责收发协议
 */
class MinigameFakeLoginService : public littleB::SyncServiceInterface<C2R_Login, R2C_Login>{
public:
//    MinigameFakeLoginService(littleB::SyncRedisWrapper& redisWrapper, littleB::RoleinfoManager& roleManager)
//        : redis_wrapper_(redisWrapper), role_manager_(roleManager) {}
    MinigameFakeLoginService(littleB::SyncRedisWrapper& redisWrapper) : redis_wrapper_(redisWrapper) {}
    R2C_Login operator()(RoleInfo& role, const C2R_Login& request) override;

    std::shared_ptr<RoleInfo> PullRoleInfoFromDB(const std::string& username);

private:
    littleB::SyncRedisWrapper &redis_wrapper_;
//    littleB::RoleinfoManager &role_manager_;
};

#endif  // LITTLEB_MINIGAME_LOGIN_SERVICE_H
