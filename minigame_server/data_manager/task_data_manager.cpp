//
// Created by xinnjie on 2019/9/28.
//

#include "task_data_manager.h"
bool TaskDataManager::PullTaskInfoFromDB(int32_t gid) {
    auto reply = redis_wrapper_.RedisCommand("get __taskinfo_%s", std::to_string(gid).c_str());
    if (reply->type == REDIS_REPLY_NIL) {
        SPDLOG_WARN("no taskinfo of gid={}", gid);
        return false;
    }
    DBTaskInfo task_info;
    task_info.ParseFromArray(reply->str, reply->len);
    SPDLOG_INFO("pull task_info from DB | gid={} | task_info={}", gid, task_info.ShortDebugString());
    if (task_infos_.count(gid) != 0) {
        SPDLOG_WARN("there already have been task_info of gid={}, update task_info | old_task_info={} | new_task_info={}",
                    gid, task_infos_[gid].ShortDebugString(), task_info.ShortDebugString());
        return false;
    }
    return task_infos_.insert({gid, task_info}).second;
}
bool TaskDataManager::PushTaskInfoToDB(int32_t gid) {
    auto iter = task_infos_.find(gid);
    if (iter == task_infos_.cend()) {
        return false;
    }
    std::string buf;
    iter->second.SerializeToString(&buf);
    auto reply = redis_wrapper_.RedisCommand("set __taskinfo_%s %b", std::to_string(gid).c_str(), buf.data(), buf.size());
    if (reply->type != REDIS_REPLY_STATUS) {
        SPDLOG_WARN("push taskinfo failed | gid={}", gid);
        return false;
    }
    SPDLOG_INFO("push to DB succeed | taskinfo={}", iter->second.ShortDebugString());
    return true;
}
DBTaskInfo& TaskDataManager::GetTaskInfo(int32_t gid) {
//    assert(task_infos_.count(gid) > 0);
    return task_infos_[gid];
}
bool TaskDataManager::RemoveTaskInfo(int32_t gid) {
    return task_infos_.erase(gid) > 0;
}

bool TaskDataManager::TaskInfoExist(int32_t gid) {
    return task_infos_.count(gid) > 0;
}
