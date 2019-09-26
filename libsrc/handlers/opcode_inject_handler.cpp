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
folly::Future<folly::Unit> OpcodeInjectHandler::write(Context *ctx, std::pair<uint32_t, std::unique_ptr<folly::IOBuf>> msg) {
    std::unique_ptr<folly::IOBuf> buf = std::move(msg.second);
    assert(buf->headroom() >= OPCODE_SIZE);
    *(buf->writableData() - sizeof(uint32_t)) = msg.first;
    buf->prepend(OPCODE_SIZE);
    return ctx->fireWrite(std::move(buf));
}
}