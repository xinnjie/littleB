//
// Created by xinnjie on 2019/9/12.
//

#ifndef LITTLEB_EXAMPLE_ASYNC_SERVICE_H
#define LITTLEB_EXAMPLE_ASYNC_SERVICE_H
#include "service_interface.h"
#include <boost/pointer_cast.hpp>
#include "example_service.pb.h"
class ExampleAsyncService : public littleB::AsyncServiceInterface<ExampleReq, ExampleResp> {
public:
    ExampleAsyncService(littleB::SyncRedisWrapper& redisWrapper);
    folly::Future<ExampleResp> operator()(RoleInfo& role, const ExampleReq& request) override;
};


#endif  // LITTLEB_EXAMPLE_ASYNC_SERVICE_H
