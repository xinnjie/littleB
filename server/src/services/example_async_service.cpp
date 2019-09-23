//
// Created by xinnjie on 2019/9/12.
//

#include "example_async_service.h"

using namespace littleB;

//
//// TODO 代码生成器：用户只需要完成该方法
//folly::Future<ExampleService::ExampleRespPtr> ExampleService::DoCall(ExampleService::ExampleReqPtr req) {
//    uint32_t example_uint = req->example_uint();
//    auto *resp = new ExampleResp();
//    RetInfo *ret_info = resp->mutable_ret();
//    ret_info->set_ret_code(0);
//    ret_info->set_ret_msg("succeed");
//    return folly::makeFuture(ExampleRespPtr(resp));
//}
