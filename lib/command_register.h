//
// Created by xinnjie on 2019/9/23.
//

#ifndef LITTLEB_COMMAND_REGISTER_H
#define LITTLEB_COMMAND_REGISTER_H
#include <functional>
#include <map>
#include <memory>

#include <folly/futures/Future.h>
#include <google/protobuf/message.h>
#include <boost/pointer_cast.hpp>

#include "role_info.h"
/**
 * use case:
 * Service class *
 *  1. Define Service class:
 *  class ExampleSyncService : public littleB::SyncServiceInterface<ExampleReq, ExampleResp>{
 *  public:
 *     ExampleResp operator()(RoleInfo& role, const ExampleReq& request) override;
 *  }
 *  2. Register to a CommandManager
 *  RegisterCommand<ExampleSyncService>(register_manager, cmd_id)
 *
 *
 * Service function *
 *  1. or Define Service function
 *  ExampleResp SomeService(RoleInfo& role, const ExampleReq& request);
 *  2. Register to a CommandManager
 *  RegisterCommand(register_manager, cmd_id, SomeService)
 *
};
 */
namespace littleB {
// 因为需要使用类的多态，所以指针类型是必须的
using MessagePtr = std::unique_ptr<google::protobuf::Message>;

using InternalSyncServiceType = std::function<MessagePtr(RoleInfo &, const google::protobuf::Message &)>;
using InternalAsyncServiceType =
    std::function<folly::Future<MessagePtr>(RoleInfo &, const google::protobuf::Message &)>;

class CommandManager {
public:
    /* 不要使用CommandManager::RegisterXXXCmd, 而应该使用RegisterXXXCommand */
    bool RegisterSyncCmd(uint32_t cmd_id, const InternalSyncServiceType &func);
    bool RegisterAsyncCmd(uint32_t cmd_id, const InternalAsyncServiceType &func);
    bool IsValidCmd(uint32_t cmd_id) const;
    MessagePtr RunSyncService(uint32_t cmd_id, RoleInfo &role, const google::protobuf::Message &request);

    folly::Future<MessagePtr> RunAsyncService(uint32_t cmd_id, RoleInfo &role,
                                              const google::protobuf::Message &request);

private:
    std::map<uint32_t, InternalSyncServiceType> sync_services_;
    std::map<uint32_t, InternalAsyncServiceType> async_services_;
};

template <typename ReqT, typename RspT>
InternalSyncServiceType SyncServiceDecorator(std::function<RspT(RoleInfo &, const ReqT &)> sync_service) {
    auto sync_service2 = [sync_service](
                             RoleInfo &role,
                             const google::protobuf::Message &req) -> std::unique_ptr<google::protobuf::Message> {
        // TODO 为了好看的 API 多了一次拷贝，但是感觉很难优化，因为本身 rsp 不是在堆上的
        const auto &derived_req = dynamic_cast<const ReqT &>(req);
        RspT derived_rsp = sync_service(role, derived_req);
        auto ptr = std::unique_ptr<RspT>(derived_rsp.New());
        ptr->CopyFrom(derived_rsp);
        return boost::dynamic_pointer_cast<google::protobuf::Message>(std::move(ptr));
    };
    return sync_service2;
}

template <typename T>
InternalSyncServiceType SyncServiceClassDecorator() {
    auto sync_service2 = [](RoleInfo &role, const google::protobuf::Message &req) -> MessagePtr {
        using ReqT = typename T::RequestType;
        using RspT = typename T::ResponseType;
        T sync_service{};
        // TODO 为了好看的 API 多了一次拷贝，但是感觉很难优化，因为本身 rsp 不是在堆上的
        const auto &derived_req = dynamic_cast<const ReqT &>(req);
        RspT derived_rsp = sync_service(role, derived_req);
        auto ptr = std::unique_ptr<RspT>(derived_rsp.New());
        ptr->CopyFrom(derived_rsp);
        return boost::dynamic_pointer_cast<google::protobuf::Message>(std::move(ptr));
    };
    return sync_service2;
}

template <typename T>
InternalAsyncServiceType AsyncServiceClassDecorator() {
    auto async_service2 = [](RoleInfo &role, const google::protobuf::Message &req) -> folly::Future<MessagePtr> {
        using ReqT = typename T::RequestType;
        using RspT = typename T::ResponseType;
        T async_service{};
        const auto &derived_req = dynamic_cast<const ReqT &>(req);
        folly::Future<RspT> rsp_future = async_service(role, derived_req);
        folly::Future<MessagePtr> internal_rsp_future =
            std::move(rsp_future).thenValue([](const RspT &rsp) -> folly::Future<MessagePtr> {
                std::unique_ptr<RspT> rsp_ptr = std::make_unique<RspT>();
                rsp_ptr->CopyFrom(rsp);
                return boost::dynamic_pointer_cast<google::protobuf::Message>(std::move(rsp_ptr));
            });
        return std::move(internal_rsp_future);
    };
    return async_service2;
}

template <typename ReqT, typename RspT,
          typename std::enable_if<std::is_base_of<google::protobuf::Message, ReqT>::value>::type = 0,
          typename std::enable_if<std::is_base_of<google::protobuf::Message, RspT>::value>::type = 0>
bool RegisterSyncCommand(CommandManager &register_manager, uint32_t cmd_id,
                         std::function<RspT(RoleInfo &, const ReqT &)> sync_service) {
    return register_manager.RegisterSyncCmd(cmd_id, SyncServiceDecorator(sync_service));
}

template <typename T>
bool RegisterSyncCommand(CommandManager &register_manager, uint32_t cmd_id) {
    return register_manager.RegisterSyncCmd(cmd_id, SyncServiceClassDecorator<T>());
}
template <typename T>
bool RegisterAsyncCommand(CommandManager &register_manager, uint32_t cmd_id) {
    return register_manager.RegisterAsyncCmd(cmd_id, AsyncServiceClassDecorator<T>());
}
}  // namespace littleB

#endif  // LITTLEB_COMMAND_REGISTER_H
