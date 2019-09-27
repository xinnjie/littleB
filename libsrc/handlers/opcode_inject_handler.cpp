//
// Created by xinnjie on 2019/9/25.
//

#include "opcode_inject_handler.h"
#include <spdlog/spdlog.h>
namespace littleB {

void OpcodeInjectHandler::read(Context *ctx, std::unique_ptr<folly::IOBuf> msg) {
    uint32_t cmd_id = 0;
    if (OPCODE_SIZE == 4) {
        cmd_id = *reinterpret_cast<const uint32_t *>(msg->data());
        msg->advance(sizeof(uint32_t));
    } else {
        cmd_id = *reinterpret_cast<const uint16_t *>(msg->data());
        msg->advance(sizeof(uint16_t));
    }
    SPDLOG_TRACE("message incomming cmd_id={}", cmd_id);
    ctx->fireRead(std::make_pair(cmd_id, std::move(msg)));
}
folly::Future<folly::Unit> OpcodeInjectHandler::write(Context *ctx,
                                                      std::pair<uint32_t, std::unique_ptr<folly::IOBuf>> msg) {
    std::unique_ptr<folly::IOBuf> buf = std::move(msg.second);
    assert(buf->headroom() >= OPCODE_SIZE);
    if (OPCODE_SIZE == 4) {
        *(reinterpret_cast<uint32_t *>(buf->writableData() - sizeof(uint32_t))) = msg.first;
    } else {
        *(reinterpret_cast<uint16_t *>(buf->writableData() - sizeof(uint16_t))) = static_cast<uint16_t>(msg.first);
    }
    buf->prepend(OPCODE_SIZE);
    return ctx->fireWrite(std::move(buf));
}
}