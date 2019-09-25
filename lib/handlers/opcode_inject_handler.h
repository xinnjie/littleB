//
// Created by xinnjie on 2019/9/25.
//

#ifndef LITTLEB_OPCODE_INJECT_HANDLER_H
#define LITTLEB_OPCODE_INJECT_HANDLER_H
#include <wangle/channel/Handler.h>
#include "common_def.h"

namespace littleB {
class OpcodeInjectHandler
    : public wangle::InboundHandler<std::unique_ptr<folly::IOBuf>, std::pair<uint32_t, std::unique_ptr<folly::IOBuf>>> {
public:
    void read(Context* ctx, std::unique_ptr<folly::IOBuf> msg) override;
};
}  // namespace littleB
#endif  // LITTLEB_OPCODE_INJECT_HANDLER_H
