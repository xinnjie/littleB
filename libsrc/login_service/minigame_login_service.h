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
 * Fake: 并不是直接收发包的 service, 登陆协议由 role injector直接接管，MinigameFakeLoginService 被 role injector 调用
 */
class MinigameFakeLoginService : public littleB::SyncServiceInterface<LoginReq, LoginRsp>{
public:
    MinigameFakeLoginService(littleB::SyncRedisWrapper& redisWrapper, littleB::RoleinfoManager& roleManager)
        : redis_wrapper_(redisWrapper), role_manager_(roleManager) {}
//    explicit MinigameFakeLoginService(littleB::SyncRedisWrapper& redisWrapper) : redis_wrapper_(redisWrapper) {}
    LoginRsp operator()(RoleInfo&, const LoginReq& request) override;
private:
    littleB::SyncRedisWrapper &redis_wrapper_;
    littleB::RoleinfoManager &role_manager_;
};

#endif  // LITTLEB_MINIGAME_LOGIN_SERVICE_H
