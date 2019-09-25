//
// Created by xinnjie on 2019/9/25.
//

#include "opcode_inject_handler.h"
namespace littleB {

void OpcodeInjectHandler::read(Context *ctx, std::unique_ptr<folly::IOBuf> msg) {
    uint32_t cmd_id = *reinterpret_cast<const uint32_t *>(msg->data());
    msg->advance(sizeof(uint32_t));
    ctx->fireRead(std::make_pair(cmd_id, std::move(msg)));
}
}