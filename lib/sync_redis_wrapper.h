//
// Created by xinnjie on 2019/9/26.
//

#ifndef LITTLEB_SYNC_REDIS_WRAPPER_H
#define LITTLEB_SYNC_REDIS_WRAPPER_H
#include <hiredis.h>
#include <spdlog/spdlog.h>
#include <cstdlib>
namespace littleB {
/**
 * 同步方式的 redis 接入，有点挫
 * TODO 熟悉 wangle 的 event loop 后使用接入异步的 redis 调用
 */
using RedisContextPtr = std::unique_ptr<redisContext, decltype(&redisFree)>;
using RedisReplyPtr = std::unique_ptr<redisReply, decltype(&freeReplyObject)>;
class SyncRedisWrapper {
public:
    redisContext &Connect(std::string hostname, int port, timeval timeout);
    SyncRedisWrapper() : redis_ptr_(nullptr, &redisFree) {}

    template<typename... ArgTypes>
    RedisReplyPtr RedisCommand(const std::string& format, ArgTypes... args) {
        assert(redis_ptr_->err == 0);
        redisReply *reply = reinterpret_cast<redisReply*>(redisCommand(redis_ptr_.get(), format.c_str(), args...));
        RedisReplyPtr reply_ptr = RedisReplyPtr(reply, &freeReplyObject);
        return reply_ptr;
    }
private:
    //    timeval timeout_;
    RedisContextPtr redis_ptr_;
};
}  // namespace littleB
#endif  // LITTLEB_SYNC_REDIS_WRAPPER_H
