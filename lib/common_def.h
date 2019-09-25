//
// Created by xinnjie on 2019/9/25.
//

#ifndef LITTLEB_COMMON_DEF_H
#define LITTLEB_COMMON_DEF_H
#include <folly/io/IOBuf.h>
#include <google/protobuf/message.h>
#include <utility>
#include <tuple>
#include "role_info.h"

namespace littleB {
using MessagePtr = std::unique_ptr<google::protobuf::Message>;
using CmdMessagePair = std::pair<uint32_t, MessagePtr>;
using CmdBufPair = std::pair<uint32_t, std::unique_ptr<folly::IOBuf>>;
using ServiceTuple = std::tuple<uint32_t, std::shared_ptr<RoleInfo>, MessagePtr>;
}  // namespace littleB
#endif  // LITTLEB_COMMON_DEF_H
