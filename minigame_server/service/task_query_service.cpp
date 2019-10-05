//
// Created by xinnjie on 2019/9/26.
//

#include "task_query_service.h"

TaskQueryRsp TaskQueryService::operator()(RoleInfo &role, const TaskQueryReq &request) {
    TaskQueryRsp rsp;
    rsp.set_rpcid(request.rpcid());
    TaskQueryRsp::ErrorCode ret = TaskQueryRsp::SUCCEED;


    int32_t gid = role.basic_info().player_id();
//    task_data_manager_.PullTaskInfoFromDB(gid);
    if (!task_data_manager_.PullTaskInfoFromDB(gid)) {
        ret = TaskQueryRsp::UNKOWN_EORROR;
        rsp.set_error(ret);
        rsp.set_message(TaskQueryRsp::ErrorCode_Name(ret));
        return rsp;

    }
    auto &taskinfo = task_data_manager_.GetTaskInfo(gid);
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
    rsp.set_position_x(taskinfo.position_x());
    rsp.set_position_y(taskinfo.position_y());
    rsp.set_error(ret);
    rsp.set_message(TaskQueryRsp::ErrorCode_Name(ret));
    return rsp;
}
