//
// Created by xinnjie on 2019/10/5.
//

#include "erase_role_service.h"

EraseRoleRsp EraseRoleService::operator()(RoleInfo &role, const EraseRoleReq &request) {
    EraseRoleRsp rsp;
    rsp.set_rpcid(request.rpcid());
    EraseRoleRsp::ErrorCode ret = EraseRoleRsp::SUCCEED;

    role.clear_basic_info();
    role.clear_progress();
    if (!role_manager_.PushRoleToDB(role)) {
        ret = EraseRoleRsp::UNKOWN_EORROR;
    }

    rsp.set_error(ret);
    rsp.set_message(EraseRoleRsp::ErrorCode_Name(ret));
    return rsp;

}
