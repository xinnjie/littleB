//
// Created by xinnjie on 2019/9/23.
//

#ifndef LITTLEB_COMMAND_REGISTER_H
#define LITTLEB_COMMAND_REGISTER_H
#include <functional>
#include <map>
#include <memory>

#include <google/protobuf/message.h>
#include <boost/pointer_cast.hpp>

#include "role_info.h"
namespace littleB {
// 因为需要使用类的多态，所以指针类型是必须的
using InternalSyncServiceType =
    std::function<std::unique_ptr<google::protobuf::Message>(RoleInfo &, const google::protobuf::Message &)>;

using SyncServiceType = std::function<google::protobuf::Message(RoleInfo &, const google::protobuf::Message &)>;

class CommandRegisterManager {
public:
    bool RegisterCmd(uint32_t cmd_id, const InternalSyncServiceType &func);

private:
    std::map<uint32_t, InternalSyncServiceType> sync_handlers_;
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
    auto sync_service2 = [](RoleInfo &role,
                            const google::protobuf::Message &req) -> std::unique_ptr<google::protobuf::Message> {
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

template <typename ReqT, typename RspT>
bool RegisterCommand(CommandRegisterManager &register_manager, uint32_t cmd_id,
                     std::function<RspT(RoleInfo &, const ReqT &)> sync_service) {
    register_manager.RegisterCmd(cmd_id, SyncServiceDecorator(sync_service));
}

template <typename T>
bool RegisterCommand(CommandRegisterManager &register_manager, uint32_t cmd_id) {
    register_manager.RegisterCmd(cmd_id, SyncServiceClassDecorator<T>());
}
}  // namespace littleB

#endif  // LITTLEB_COMMAND_REGISTER_H
