//
// Created by xinnjie on 2019/9/24.
//
#include "CmdMessageSerializeHandler.h"
#include <folly/ExceptionWrapper.h>
#include <folly/futures/Future.h>
#include <spdlog/spdlog.h>
#include <wangle/channel/Handler.h>
#include <iostream>
namespace littleB {

////从IObuf中读取数据 反序列化到对应的 pb 结构
//void CmdMessageSerializeHandler::read(Context* ctx, std::unique_ptr<folly::IOBuf> msg) {
//    rpc::codec::RpcMessage in;
////    uint32_t cmd
////    std::unique_ptr<google::protobuf::Message> pb = reflection_manager_
//    in.ParseFromArray((void*)msg->data(), msg->length());
//
//    //        printf("[ServerSerializeHandler] READ: \n");
//    //        printf("------- in id       = %d \n",in.id());
//    //        printf("------- in request  = %s \n",in.request().c_str());
//    //        printf("------- in response = %s \n",in.response().c_str());
//
//    ctx->fireRead(std::move(in));
//}
//
////读取到EOF 说明对端关闭了连接
//void CmdMessageSerializeHandler::readEOF(Context* ctx) {
//    std::cout << "[ServerSerializeHandler] -- read EOF" << std::endl;
//    Handler::readEOF(ctx);
//}
//
////读取数据出现异常
//void CmdMessageSerializeHandler::readException(Context* ctx, folly::exception_wrapper e) {
//    std::cout << "[ServerSerializeHandler] -- read Exception:" << exceptionStr(e) << std::endl;
//    Handler::readException(ctx, e);
//}
//
////从RpcMessage中写数据 到 IOBuf中 利用protobuf进行序列化 得到二进制的RpcMessage
//folly::Future<folly::Unit> CmdMessageSerializeHandler::write(Context* ctx, rpc::codec::RpcMessage msg) {
//    std::string outstr;
//
//    //将RpcMesage序列化写入到string中
//    msg.SerializePartialToString(&outstr);
//
//    //      std::cout<<"[ServerSerializeHandler WRITE]:"<<std::endl;
//    //      std::cout<<outstr<<std::endl;
//
//    return ctx->fireWrite(folly::IOBuf::copyBuffer(outstr));
//}
//
////写入数据出现异常
//folly::Future<folly::Unit> CmdMessageSerializeHandler::writeException(Context* ctx, folly::exception_wrapper e) {
//    std::cout << "[ServerSerializeHandler] -- write Exception:" << exceptionStr(e) << std::endl;
//    return Handler::writeException(ctx, e);
//}
//
//folly::Future<folly::Unit> CmdMessageSerializeHandler::close(Context* ctx) {
//    std::cout << "[ServerSerializeHandler] -- pipeline closed" << std::endl;
//    return Handler::close(ctx);
//}

}