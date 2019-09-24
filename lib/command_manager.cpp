//
// Created by xinnjie on 2019/9/23.
//
#include "command_manager.h"
#include <cassert>

namespace littleB {
bool CommandManager::RegisterSyncCmd(uint32_t cmd_id, const InternalSyncServiceType& func) {
    assert(!IsValidCmd(cmd_id));
    auto ret = sync_services_.insert(std::make_pair(cmd_id, func));
    return ret.second;
}
MessagePtr CommandManager::RunSyncService(uint32_t cmd_id, RoleInfo& role,
                                                                          const google::protobuf::Message& request) {
    auto iter = sync_services_.find(cmd_id);
    assert(iter != sync_services_.cend());
    auto sync_service = iter->second;
    return sync_service(role, request);
}
bool CommandManager::IsValidCmd(uint32_t cmd_id) const {
    return sync_services_.count(cmd_id) > 0 || async_services_.count(cmd_id) > 0;
}
bool CommandManager::RegisterAsyncCmd(uint32_t cmd_id, const InternalAsyncServiceType& func) {
    assert(!IsValidCmd(cmd_id));
    auto ret = async_services_.insert(std::make_pair(cmd_id, func));
    return ret.second;
}
folly::Future<MessagePtr> CommandManager::RunAsyncService(uint32_t cmd_id, RoleInfo& role, const google::protobuf::Message &request) {
    auto iter = async_services_.find(cmd_id);
    assert(iter != async_services_.cend());
    auto async_service = iter->second;
    return async_service(role, request);
}
}  // namespace littleB