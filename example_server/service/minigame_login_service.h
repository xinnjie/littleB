//
// Created by xinnjie on 2019/9/26.
//

#ifndef LITTLEB_MINIGAME_LOGIN_SERVICE_H
#define LITTLEB_MINIGAME_LOGIN_SERVICE_H
#include "service_interface.h"
#include "sync_redis_wrapper.h"
#include "login.pb.h"
class MinigameLoginService : public littleB::SyncServiceInterface<C2R_Login, R2C_Login>{
public:
    explicit MinigameLoginService(littleB::SyncRedisWrapper& redis_wrapper):redis_wrapper_(redis_wrapper) {}
    R2C_Login operator()(RoleInfo& role, const C2R_Login& request) override;

private:
    littleB::SyncRedisWrapper &redis_wrapper_;
};

#endif  // LITTLEB_MINIGAME_LOGIN_SERVICE_H
