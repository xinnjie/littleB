//
// Created by xinnjie on 2019/9/28.
//

#ifndef LITTLEB_TASK_DATA_MANAGER_H
#define LITTLEB_TASK_DATA_MANAGER_H
#include <unordered_map>
#include "sync_redis_wrapper.h"
#include "task_db.pb.h"

class TaskDataManager {
public:
    explicit TaskDataManager(littleB::SyncRedisWrapper &redisWrapper) : redis_wrapper_(redisWrapper) {}

    bool PullTaskInfoFromDB(int32_t gid);

    bool PushTaskInfoToDB(int32_t gid);

    DBTaskInfo &GetTaskInfo(int32_t gid);

    bool RemoveTaskInfo(int32_t gid);

private:
    littleB::SyncRedisWrapper &redis_wrapper_;
    std::unordered_map<int32_t, DBTaskInfo> task_infos_;
};

#endif  // LITTLEB_TASK_DATA_MANAGER_H
