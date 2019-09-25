//
// Created by xinnjie on 2019/9/26.
//

#include "sync_redis_wrapper.h"
redisContext& littleB::SyncRedisWrapper::Connect(std::string hostname, int port, timeval timeout) {
    redisContext *redis_context = redisConnectWithTimeout(hostname.c_str(), port, timeout);
    redis_ptr_ = RedisContextPtr(redis_context, &redisFree);
    return  *redis_context;
}
