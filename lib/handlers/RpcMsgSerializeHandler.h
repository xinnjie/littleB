//
// Created by chijinxin on 19-4-22.
//

#ifndef WANGLE_RPCMSGSERIALIZEHANDLER_H
#define WANGLE_RPCMSGSERIALIZEHANDLER_H

#include <wangle/channel/Handler.h>


class RpcMsgSerializeHandler
           : public wangle::Handler< std::unique_ptr<folly::IOBuf>,
                                     rpc::codec::RpcMessage,
                                     rpc::codec::RpcMessage,
                                     std::unique_ptr<folly::IOBuf> >
{
public:
    //从IObuf中读取数据 到  RpcMessage中 利用protobuf进行反序列化 得到 rpc::codec::RpcMessage
    void read(Context *ctx, std::unique_ptr<folly::IOBuf> msg) override;

    //对端关闭连接
    void readEOF(Context *ctx) override;

    //读取数据发生异常
    void readException(Context *ctx, folly::exception_wrapper e) override;

    //从RpcMessage中写数据 到 IOBuf中 利用protobuf进行序列化 得到二进制的RpcMessage
    folly::Future<folly::Unit> write(Context *ctx, rpc::codec::RpcMessage msg) override;

    //写入数据出现异常
    folly::Future<folly::Unit> writeException(Context *ctx, folly::exception_wrapper e) override;

    //关闭pipeline
    folly::Future<folly::Unit> close(Context *ctx) override;
};


#endif //WANGLE_RPCMSGSERIALIZEHANDLER_H
