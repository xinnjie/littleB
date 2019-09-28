//
// Created by xinnjie on 2019/9/26.
//

#include "task_query_service.h"
TaskQueryRsp TaskQueryService::operator()(RoleInfo& role, const TaskQueryReq& request) {
    int32_t gid = role.basic_info().player_id();
    auto &taskinfo = task_data_manager_.GetTaskInfo(gid);

    TaskQueryRsp rsp;
    rsp.set_rpcid(request.rpcid());

    TaskQueryRsp::ErrorCode ret = TaskQueryRsp::SUCCEED;

    for (auto &item : taskinfo.tasks()) {
        switch (item.second) {
            case DBTaskInfo::START: {
                rsp.mutable_get_tasks()->Add(item.first);
                break;
            }
            case DBTaskInfo::FINISH: {
                rsp.mutable_done_tasks()->Add(item.first);
                break;
            }
        }
    }
    rsp.set_error(ret);
    rsp.set_message(TaskQueryRsp::ErrorCode_Name(ret));
    return rsp;
}
