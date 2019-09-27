//
// Created by xinnjie on 2019/9/25.
//

#ifndef LITTLEB_COMMON_DEF_H
#define LITTLEB_COMMON_DEF_H
#include <folly/io/IOBuf.h>
#include <google/protobuf/message.h>
#include <tuple>
#include <utility>
#include "role.pb.h"

namespace littleB {
using MessagePtr = std::unique_ptr<google::protobuf::Message>;
using CmdMessagePair = std::pair<uint32_t, MessagePtr>;
using CmdBufPair = std::pair<uint32_t, std::unique_ptr<folly::IOBuf>>;
using ServiceTuple = std::tuple<uint32_t, std::shared_ptr<RoleInfo>, MessagePtr>;
// TODO  按照PKG_LENGTH_FIELD_SIZE、OPCODE_SIZE调整 写入网络包中前置字段大小
constexpr uint32_t PKG_LENGTH_FIELD_SIZE = 2;
constexpr uint32_t OPCODE_SIZE = 2;

static_assert(OPCODE_SIZE == 4 || OPCODE_SIZE == 2, "opcode should be uint32_t or uint16_t");
}  // namespace littleB
#endif  // LITTLEB_COMMON_DEF_H
