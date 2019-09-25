//
// Created by xinnjie on 2019/9/24.
//

#ifndef LITTLEB_CMD_MESSAGE_SERIALIZE_HANDLER_H
#define LITTLEB_CMD_MESSAGE_SERIALIZE_HANDLER_H
#include <google/protobuf/message.h>
#include <wangle/channel/Handler.h>
#include "common_def.h"
#include "data_manager/pb_reflection_manager.h"
namespace littleB {
/**
 * 上行 <cmd_id, unique_IOBuf> -> <cmd_id, Message>
 * 下行 <cmd_id, Message> -> <cmd_id, unique_IOBuf>
 */
class CmdMessageSerializeHandler : public wangle::Handler<CmdBufPair, CmdMessagePair, CmdMessagePair, CmdBufPair> {
public:
    explicit CmdMessageSerializeHandler(PbReflectionManager &reflectionManager) : reflection_manager_(reflectionManager) {}
    //从 IObuf 中读取数据 到  Message 中, 利用 protobuf 进行反序列化 std::unique_ptr<google::protobuf::Message>
    void read(Context *ctx, CmdBufPair msg) override;
    //对端关闭连接
    void readEOF(Context *ctx) override;

    //读取数据发生异常
    void readException(Context *ctx, folly::exception_wrapper e) override;

    //序列化 Message 到 IOBuf中
    folly::Future<folly::Unit> write(Context *ctx, CmdMessagePair msg) override;
    //写入数据出现异常
    folly::Future<folly::Unit> writeException(Context *ctx, folly::exception_wrapper e) override;

    //关闭pipeline
    folly::Future<folly::Unit> close(Context *ctx) override;

private:
    PbReflectionManager &reflection_manager_;
};
}  // namespace littleB
#endif  // LITTLEB_CMD_MESSAGE_SERIALIZE_HANDLER_H
