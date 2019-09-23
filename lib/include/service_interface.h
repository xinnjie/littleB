//
// Created by xinnjie on 2019/9/12.
//

#ifndef LITTLEB_SERVICE_INTERFACE_H
#define LITTLEB_SERVICE_INTERFACE_H
#include <google/protobuf/message.h>
#include <wangle/service/Service.h>
#include <memory>

#include "role_info.h"
namespace littleB {
struct MethodData {
    std::unique_ptr<google::protobuf::MethodDescriptor> descriptor;
    std::unique_ptr<google::protobuf::Message> request;
    std::unique_ptr<google::protobuf::Message> response;
};

/* 为了多态使用指针作为 Service 的输入/输出 */
using RequestPtr = std::unique_ptr<google::protobuf::Message>;
using ResponsePtr = std::unique_ptr<google::protobuf::Message>;
// TODO 不同的 response/request 类型可能需要强转，netty 应该也是这么干的
// TODO Service 之间可以互相调用，这一点决定了 Service 复用性极强
// TODO AsyncService 的概念基本和 wangle::Service 一致。
// Service 返回后的 Future 需要紧随其后的 dispatcher 需要待 Future 被填充后，投送下行 pipeline
/* LittleBAsyncService 异步Service，返回 Future */

class AsyncServiceInterface {
public:
    explicit AsyncServiceInterface(const std::shared_ptr<RoleInfo> &role_ptr) : role_ptr_(role_ptr) {}
    virtual folly::Future<ResponsePtr> operator()(RequestPtr request) = 0;

protected:
    MethodData method_data_{};

private:
    std::weak_ptr<RoleInfo>
        role_ptr_;  // TODO 如果在异步过程中 role 被销毁怎么办, 看起来只能用 weak_ptr 这样 Service 才能感知 role 被销毁
};

/* SyncService 为同步 Service */
// TODO Syncervice 同样和 wangle 中Service 和 pipeline 完全无关。

template <typename ReqT, typename RspT>
class SyncServiceInterface {
public:
    virtual RespT operator()(RequestPtr request) = 0;
    virtual ~SyncServiceInterface() = default;

    MethodData method_data_{};
};

}  // namespace littleB

#endif  // LITTLEB_SERVICE_INTERFACE_H