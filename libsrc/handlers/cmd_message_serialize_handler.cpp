//
// Created by xinnjie on 2019/9/24.
//
#include "cmd_message_serialize_handler.h"
#include <folly/ExceptionWrapper.h>
#include <folly/futures/Future.h>
#include <spdlog/spdlog.h>
#include <wangle/channel/Handler.h>
namespace littleB {

//从 IObuf 中读取数据 反序列化到对应的 pb 结构
void CmdMessageSerializeHandler::read(Context* ctx, CmdBufPair msg) {
    uint32_t cmd_id = msg.first;
    std::unique_ptr<folly::IOBuf> buf = std::move(msg.second);
    std::unique_ptr<google::protobuf::Message> pb =
        std::unique_ptr<google::protobuf::Message>(reflection_manager_.GetRequestReflection(cmd_id).New());
    if (!pb->ParseFromArray(reinterpret_cast<const void*>(buf->data()), buf->length())) {
        SPDLOG_ERROR("[CmdMessageSerializeHandler] -- parse pb error | cmd={}", cmd_id);
        ctx->fireReadEOF();  // TODO 发回 parser error 的错误码
    } else {
        SPDLOG_INFO("[CmdMessageSerializeHandler] -- incoming cmd={} | request={}", cmd_id, pb->ShortDebugString());
        ctx->fireRead(std::make_pair(cmd_id, std::move(pb)));
    }
}

//读取到EOF 说明对端关闭了连接
void CmdMessageSerializeHandler::readEOF(Context* ctx) {
    SPDLOG_INFO("[CmdMessageSerializeHandler] -- read EOF");
    Handler::readEOF(ctx);
}

//读取数据出现异常
void CmdMessageSerializeHandler::readException(Context* ctx, folly::exception_wrapper e) {
    SPDLOG_WARN("[CmdMessageSerializeHandler] -- read Exception={}", exceptionStr(e).toStdString());
    Handler::readException(ctx, e);
}
folly::Future<folly::Unit> CmdMessageSerializeHandler::write(Context* ctx, CmdMessagePair msg) {
    auto message_ptr = std::move(msg.second);
    uint32_t cmd_id = msg.first;
    auto buffer = folly::IOBuf::create(message_ptr->ByteSizeLong());
    message_ptr->SerializeToArray(buffer->writableData(), message_ptr->ByteSizeLong());
    buffer->append(message_ptr->ByteSizeLong());
    SPDLOG_INFO("[CmdMessageSerializeHandler] -- outgoing cmd={} | response={} | message length", cmd_id,
                message_ptr->ShortDebugString(), message_ptr->ByteSizeLong());
    return ctx->fireWrite(std::make_pair(cmd_id, std::move(buffer)));
}
//写入数据出现异常
folly::Future<folly::Unit> CmdMessageSerializeHandler::writeException(Context* ctx, folly::exception_wrapper e) {
    SPDLOG_WARN("[ServerSerializeHandler] -- write Exception: {}", exceptionStr(e).toStdString());
    return Handler::writeException(ctx, e);
}

folly::Future<folly::Unit> CmdMessageSerializeHandler::close(Context* ctx) {
    SPDLOG_INFO("[ServerSerializeHandler] -- pipeline closed");
    return Handler::close(ctx);
}

}  // namespace littleB