//
// Created by xinnjie on 2019/9/28.
//

#ifndef LITTLEB_MINIGAME_REGISTER_SERVICE_H
#define LITTLEB_MINIGAME_REGISTER_SERVICE_H
#include "data_manager/roleinfo_manager.h"
#include "register.pb.h"
#include "service_interface.h"
#include "sync_redis_wrapper.h"
class MinigameRegisterService : public littleB::SyncServiceInterface<RegisterReq, RegisterRsp> {
public:
    explicit MinigameRegisterService(littleB::SyncRedisWrapper& redisWrapper) : redis_wrapper_(redisWrapper) {}
    /**
     * 注册账号信息，注册后向数据库填充空的玩家信息
     * @param role
     * @param request
     * @return 如果注册账号已存在, rsp 返回错误码 ACCOUNT_ALREADY_EXIST
     *         成功返回 SUCCEED
     */
    RegisterRsp operator()(RoleInfo& role, const RegisterReq& request) override;
private:
    littleB::SyncRedisWrapper& redis_wrapper_;
};
#endif  // LITTLEB_MINIGAME_REGISTER_SERVICE_H
