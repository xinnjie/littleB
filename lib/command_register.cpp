//
// Created by xinnjie on 2019/9/23.
//
#include <cassert>
#include "command_register.h"
namespace littleB {
bool CommandRegisterManager::RegisterCmd(uint32_t cmd_id, const InternalSyncServiceType& func) {
    assert(sync_handlers_.count(cmd_id) == 0);
    if (sync_handlers_.count(cmd_id) > 0) {
        return false;
    }
    auto ret = sync_handlers_.insert(std::make_pair(cmd_id, func));
    return ret.second;
}
}  // namespace littleB