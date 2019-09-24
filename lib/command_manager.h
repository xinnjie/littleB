//
// Created by xinnjie on 2019/9/23.
//

#ifndef LITTLEB_COMMAND_MANAGER_H
#define LITTLEB_COMMAND_MANAGER_H
#include <functional>
#include <map>
#include <memory>

#include <folly/futures/Future.h>
#include <google/protobuf/message.h>
#include <spdlog/spdlog.h>
#include <boost/pointer_cast.hpp>

#include "role_info.h"

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
}  // namespace littleB

#endif  // LITTLEB_COMMAND_MANAGER_H
