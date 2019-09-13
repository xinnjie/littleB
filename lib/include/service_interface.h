//
// Created by xinnjie on 2019/9/12.
//

#ifndef LITTLEB_SERVICE_INTERFACE_H
#define LITTLEB_SERVICE_INTERFACE_H
#include <google/protobuf/message.h>
#include <wangle/service/Service.h>
#include <memory>
namespace littleB {
struct MethodData {
    const google::protobuf::MethodDescriptor *descriptor;
    const google::protobuf::Message *request;
    const google::protobuf::Message *response;
};

/* 为了多态使用指针作为 Service 的输入/输出 */
using RequestPtr = std::unique_ptr<google::protobuf::Message>;
using ResponsePtr = std::unique_ptr<google::protobuf::Message>;
// TODO 不同的 response/request 类型可能需要强转，netty 应该也是这么干的
/* LittleBAsyncService 异步Service，返回 Future */
class AsyncServiceInterface : public wangle::Service<RequestPtr, ResponsePtr> {
public:
    folly::Future<ResponsePtr> operator()(RequestPtr request) override { return folly::Future<ResponsePtr>(nullptr); };

    MethodData method_data_{};
};

/* LittleBSyncService 为同步 Service */
class SyncServiceInterface {
public:
    virtual ResponsePtr operator()(RequestPtr request) = 0;
    virtual ~SyncServiceInterface() = default;

    MethodData method_data_{};
};

}  // namespace littleB

#endif  // LITTLEB_SERVICE_INTERFACE_H