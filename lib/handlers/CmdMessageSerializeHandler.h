//
// Created by xinnjie on 2019/9/24.
//

#ifndef LITTLEB_CMDMESSAGESERIALIZEHANDLER_H
#define LITTLEB_CMDMESSAGESERIALIZEHANDLER_H
#include <google/protobuf/message.h>
#include <wangle/channel/Handler.h>
#include "pb_reflection_manager.h"

namespace littleB {

class CmdMessageSerializeHandler
    : public wangle::Handler<std::unique_ptr<folly::IOBuf>, std::unique_ptr<google::protobuf::Message>,
                             std::unique_ptr<google::protobuf::Message>, std::unique_ptr<folly::IOBuf> > {
public:
    CmdMessageSerializeHandler(PbReflectionManager &reflectionManager) : reflection_manager_(reflectionManager) {}
    //从IObuf中读取数据 到  RpcMessage中 利用 protobuf 进行反序列化 std::unique_ptr<google::protobuf::Message>
    void read(Context *ctx, std::unique_ptr<folly::IOBuf> msg) override;

    //对端关闭连接
    void readEOF(Context *ctx) override;

    //读取数据发生异常
    void readException(Context *ctx, folly::exception_wrapper e) override;

    //从RpcMessage中写数据 到 IOBuf中 利用protobuf进行序列化 得到二进制的RpcMessage
    folly::Future<folly::Unit> write(Context *ctx, std::unique_ptr<google::protobuf::Message> msg) override;

    //写入数据出现异常
    folly::Future<folly::Unit> writeException(Context *ctx, folly::exception_wrapper e) override;

    //关闭pipeline
    folly::Future<folly::Unit> close(Context *ctx) override;

private:
    PbReflectionManager &reflection_manager_;

};
}  // namespace littleB
#endif  // LITTLEB_CMDMESSAGESERIALIZEHANDLER_H
