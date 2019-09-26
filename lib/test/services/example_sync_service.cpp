//
// Created by xinnjie on 2019/9/23.
//

#include "example_sync_service.h"
ExampleResp ExampleSyncService::operator()(RoleInfo& role, const ExampleReq& request) {
    ExampleResp rsp;
    rsp.mutable_ret()->set_ret_code(request.example_int());
    rsp.mutable_ret()->set_ret_msg("this is example service!");
    return rsp;
}
ExampleSyncService::ExampleSyncService(littleB::SyncRedisWrapper& redisWrapper) : SyncServiceInterface(redisWrapper) {}
