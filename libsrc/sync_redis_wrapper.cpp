//
// Created by xinnjie on 2019/9/26.
//

#include "sync_redis_wrapper.h"
#include <spdlog/spdlog.h>
redisContext& littleB::SyncRedisWrapper::Connect(std::string hostname, int port, timeval timeout) {
    redisContext *redis_context = redisConnectWithTimeout(hostname.c_str(), port, timeout);
    if (redis_context->err == 0) {
        SPDLOG_INFO("connected to redis");
    } else {
        SPDLOG_INFO("connected to redis failed");
    }
    redis_ptr_ = RedisContextPtr(redis_context, &redisFree);
    return  *redis_context;
}
