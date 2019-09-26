//
// Created by xinnjie on 2019/9/12.
//

#include "example_async_service.h"

using namespace littleB;

folly::Future<ExampleResp> ExampleAsyncService::operator()(RoleInfo &role, const ExampleReq &request) {
    ExampleResp resp;
    RetInfo *ret_info = resp.mutable_ret();
    ret_info->set_ret_code(request.example_int());
    ret_info->set_ret_msg("succeed");
    return folly::makeFuture(resp);
}
