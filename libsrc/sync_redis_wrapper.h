//
// Created by xinnjie on 2019/9/26.
//

#ifndef LITTLEB_SYNC_REDIS_WRAPPER_H
#define LITTLEB_SYNC_REDIS_WRAPPER_H
#include <hiredis.h>
#include <spdlog/spdlog.h>
#include <cstdlib>
#include <mutex>
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
    RedisReplyPtr RedisCommand(const char *format, ArgTypes... args) {
        static char buf[100];
        std::lock_guard<std::mutex> guard(redis_lock_);
        assert(redis_ptr_->err == 0);
        redisReply *reply = reinterpret_cast<redisReply*>(redisCommand(redis_ptr_.get(), format, args...));
        std::snprintf(buf, 100, format, args...);
        SPDLOG_TRACE("redis_command={} | reply_type={}", std::string(buf), reply->type);
        if (reply->type == REDIS_REPLY_STATUS) {
            SPDLOG_INFO("reply status: {}", std::string(reply->str, reply->len));
        }
        RedisReplyPtr reply_ptr = RedisReplyPtr(reply, &freeReplyObject);
        return reply_ptr;
    }
private:
    // redis 同步接口本身不是线程安全的，需要加锁
    std::mutex redis_lock_;
    RedisContextPtr redis_ptr_;
};
}  // namespace littleB
#endif  // LITTLEB_SYNC_REDIS_WRAPPER_H
