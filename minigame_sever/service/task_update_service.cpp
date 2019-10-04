//
// Created by xinnjie on 2019/9/28.
//

#include "task_update_service.h"
TaskUpdateRsp TaskUpdateService::operator()(RoleInfo& role, const TaskUpdateReq& request) {
    int32_t gid = role.basic_info().player_id();
    auto& task_info = task_data_manager_.GetTaskInfo(gid);
    TaskUpdateRsp rsp;
    rsp.set_rpcid(request.rpcid());

    TaskUpdateRsp::ErrorCode ret = TaskUpdateRsp::SUCCEED;

    auto* task_map = task_info.mutable_tasks();
    for (auto finish_task_id : request.finished_task_ids()) {
        (*task_map)[finish_task_id] = DBTaskInfo::FINISH;
    }
    for (auto get_task_id : request.get_task_ids()) {
        (*task_map)[get_task_id] = DBTaskInfo::START;
    }
    task_info.set_position_x(request.position_x());
    task_info.set_position_y(request.position_y());
    task_data_manager_.PushTaskInfoToDB(gid);
    rsp.set_error(ret);
    rsp.set_message(TaskUpdateRsp::ErrorCode_Name(ret));
    return rsp;
}
