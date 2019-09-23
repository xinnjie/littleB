//
// Created by xinnjie on 2019/9/23.
//
#include <cassert>
#include "command_register.h"
namespace littleB {
bool CommandManager::RegisterCmd(uint32_t cmd_id, const InternalSyncServiceType& func) {
    assert(sync_handlers_.count(cmd_id) == 0);
    if (sync_handlers_.count(cmd_id) > 0) {
        return false;
    }
    auto ret = sync_handlers_.insert(std::make_pair(cmd_id, func));
    return ret.second;
}
std::unique_ptr<google::protobuf::Message> CommandManager::operator()(uint32_t cmd_id, RoleInfo& role,
                                                                      const google::protobuf::Message& request) {
    assert(IsValidCmd(cmd_id));
    auto iter = sync_handlers_.find(cmd_id);
    auto sync_service = iter->second;
    return sync_service(role, request);
}
bool CommandManager::IsValidCmd(uint32_t cmd_id){
    auto iter = sync_handlers_.find(cmd_id);
    return iter != sync_handlers_.cend();
}
}  // namespace littleB