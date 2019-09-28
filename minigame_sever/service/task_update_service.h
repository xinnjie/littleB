//
// Created by xinnjie on 2019/9/28.
//

#ifndef LITTLEB_TASK_UPDATE_SERVICE_H
#define LITTLEB_TASK_UPDATE_SERVICE_H
#include <data_manager/task_data_manager.h>
#include "service_interface.h"
#include "task_progress.pb.h"
/**
 * 更新当前任务进度
 */
class TaskQueryService : public littleB::SyncServiceInterface<TaskUpdateReq, TaskUpdateRsp> {
public:
    explicit TaskQueryService(TaskDataManager& task_data_manager) : task_data_manager_(task_data_manager) {}

    /**
     * 向 task_data_manager 更新该玩家的任务信息
     * @param role
     * @param request
     * @return
     */
    TaskUpdateRsp operator()(RoleInfo& role, const TaskUpdateReq& request) override;

private:
    TaskDataManager task_data_manager_;
};

#endif  // LITTLEB_TASK_UPDATE_SERVICE_H
