//
// Created by xinnjie on 2019/9/25.
//

#include "opcode_inject_handler.h"
#include <folly/io/Cursor.h>
#include <folly/io/IOBuf.h>
#include <spdlog/spdlog.h>
namespace littleB {

using namespace folly;
void OpcodeInjectHandler::read(Context *ctx, std::unique_ptr<folly::IOBuf> msg) {
    folly::io::Cursor cursor(msg.get());
    uint32_t cmd_id = 0;
    if (OPCODE_SIZE == 4) {
        //  cmd_id = *reinterpret_cast<const uint32_t *>(msg->data());
        //  msg->trimStart(sizeof(uint32_t));
        cmd_id = cursor.readLE<uint32_t>();
        msg->trimStart(sizeof(uint32_t));
    } else {
        //  cmd_id = *reinterpret_cast<const uint16_t *>(msg->data());
        //  msg->trimStart(sizeof(uint16_t));
        cmd_id = static_cast<uint32_t>(cursor.readLE<uint16_t>());
        msg->trimStart(sizeof(uint16_t));
    }
    SPDLOG_INFO("message incomming cmd_id={}", cmd_id);
    ctx->fireRead(std::make_pair(cmd_id, std::move(msg)));
}
folly::Future<folly::Unit> OpcodeInjectHandler::write(Context *ctx,
                                                      std::pair<uint32_t, std::unique_ptr<folly::IOBuf>> msg) {
    auto opcode_buf = IOBuf::create(OPCODE_SIZE);
    opcode_buf->append(OPCODE_SIZE);
    folly::io::RWPrivateCursor c(opcode_buf.get());
    /* 客户端的命令字是自动生成的，并且比较奇怪，同一个命令的 id 假如是 n, 那么回包的 id 就是 n+1 */
    if (OPCODE_SIZE == 4) {
        c.writeLE(static_cast<uint32_t>(msg.first + 1));
    } else {
        c.writeLE(static_cast<uint16_t>(msg.first + 1));
    }
    opcode_buf->prependChain(std::move(msg.second));
    return ctx->fireWrite(std::move(opcode_buf));
}
}