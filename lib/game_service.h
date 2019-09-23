//
// Created by xinnjie on 2019/9/10.
//
#ifndef LITTLEB_GAME_SERVICE_H
#define LITTLEB_GAME_SERVICE_H

#include <google/protobuf/message.h>
#include <wangle/service/Service.h>

#include <memory>
#include <unordered_map>
#include <utility>

#include "service_interface.h"

namespace littleB {

// using CallFunc = std::function

class GameService {
public:
    using CmdId = uint32_t;
    bool RegisterSyncService(CmdId cmd_id, std::unique_ptr<SyncServiceInterface> &&service) {
        auto iter = sync_services_.emplace(std::make_pair(cmd_id, std::move(service)));  // std::forward为什么不行, forward 应该是针对模板的
        return iter.second;
    }

    bool RegisterAsyncService(CmdId cmd_id, std::unique_ptr<AsyncServiceInterface> &&service) {
        auto iter = async_services_.emplace(std::make_pair(cmd_id, std::move(service)));
        return iter.second;
    }
    // TODO 可能需要工厂，因为有状态的
    // service 之间的状态是隔离的
    std::unordered_map<uint32_t, std::unique_ptr<AsyncServiceInterface>> async_services_;
    std::unordered_map<uint32_t, std::unique_ptr<SyncServiceInterface>> sync_services_;
};
}  // namespace littleB

#endif  // LITTLEB_GAME_SERVICE_H
