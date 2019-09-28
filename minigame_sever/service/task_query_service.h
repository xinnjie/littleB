//
// Created by xinnjie on 2019/9/26.
//

#ifndef LITTLEB_TASK_QUERY_SERVICE_H
#define LITTLEB_TASK_QUERY_SERVICE_H
#include "data_manager/task_data_manager.h"
#include "service_interface.h"
#include "task_progress.pb.h"
/**
 * 查询当前进度
 */
class TaskQueryService : public littleB::SyncServiceInterface<TaskQueryReq, TaskQueryRsp> {
public:
    explicit TaskQueryService(TaskDataManager& task_data_manager) : task_data_manager_(task_data_manager) {}

    /**
     * 向 task_data_manager 查询该玩家的任务信息
     * @param role
     * @param request
     * @return
     */
    TaskQueryRsp operator()(RoleInfo& role, const TaskQueryReq& request) override;

private:
    TaskDataManager task_data_manager_;
};

#endif  // LITTLEB_TASK_QUERY_SERVICE_H
