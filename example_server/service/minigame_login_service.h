//
// Created by xinnjie on 2019/9/26.
//

#ifndef LITTLEB_MINIGAME_LOGIN_SERVICE_H
#define LITTLEB_MINIGAME_LOGIN_SERVICE_H
#include "service_interface.h"
#include "login.pb.h"
class MinigameLoginService : public littleB::SyncServiceInterface<C2R_Login, R2C_Login>{
public:
    explicit MinigameLoginService(littleB::SyncRedisWrapper& redisWrapper);
    R2C_Login operator()(RoleInfo& role, const C2R_Login& request) override;
};

#endif  // LITTLEB_MINIGAME_LOGIN_SERVICE_H
