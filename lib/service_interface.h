//
// Created by xinnjie on 2019/9/12.
//

#ifndef LITTLEB_SERVICE_INTERFACE_H
#define LITTLEB_SERVICE_INTERFACE_H
#include <google/protobuf/message.h>
#include <wangle/service/Service.h>
#include <memory>
#include "role.pb.h"
#include "sync_redis_wrapper.h"
namespace littleB {
// TODO Service 之间可以互相调用，这一点决定了 Service 复用性极强
// TODO AsyncService 的概念基本和 wangle::Service 一致。
// TODO 保证 ReqT 和 RspT都是 PB 结构
/* LittleBAsyncService 异步Service，返回 Future */
template <typename ReqT, typename RspT>
class AsyncServiceInterface {
public:
    using RequestType = ReqT;
    using ResponseType = RspT;
    AsyncServiceInterface() = default;
    virtual folly::Future<RspT> operator()(RoleInfo &role, const ReqT &request) = 0;
    virtual ~AsyncServiceInterface() = default;
};

/* SyncService 为同步 Service */
template <typename ReqT, typename RspT>
class SyncServiceInterface {
public:
    using RequestType = ReqT;
    using ResponseType = RspT;
    SyncServiceInterface() = default;
    virtual RspT operator()(RoleInfo &role, const ReqT &request) = 0;
    virtual ~SyncServiceInterface() = default;
};


}  // namespace littleB

#endif  // LITTLEB_SERVICE_INTERFACE_H