//
// Created by xinnjie on 2019/9/10.
//

#ifndef LITTLEB_COMMAND_DISTRIBUTOR_H
#define LITTLEB_COMMAND_DISTRIBUTOR_H

#include "wangle/channel/Handler.h"

using CmdPair = std::pair<uint32_t, folly::IOBuf>;
/**
 * 用于分发命令的 handler
 */
// class CommandDistributor : public wangle::HandlerAdapter<CmdPair, >{};

#endif  // LITTLEB_COMMAND_DISTRIBUTOR_H
