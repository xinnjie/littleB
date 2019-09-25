//
// Created by xinnjie on 2019/9/10.
//

#ifndef LITTLEB_COMMAND_DISTRIBUTOR_H
#define LITTLEB_COMMAND_DISTRIBUTOR_H

#include <wangle/channel/Handler.h>
#include "common_def.h"
#include "data_manager/command_manager.h"
namespace littleB {
/**
 * 用于分发命令的 handler
 */
class CommandDistributor : public wangle::HandlerAdapter<ServiceTuple, CmdMessagePair> {
public:
    explicit CommandDistributor(CommandManager& commandManager) : command_manager_(commandManager) {}
    void read(Context* ctx, ServiceTuple msg) override;

private:
    CommandManager &command_manager_;
};
}
#endif  // LITTLEB_COMMAND_DISTRIBUTOR_H
