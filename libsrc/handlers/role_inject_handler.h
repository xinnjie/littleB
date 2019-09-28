//
// Created by xinnjie on 2019/9/25.
//

#ifndef LITTLEB_ROLE_INJECT_HANDLER_H
#define LITTLEB_ROLE_INJECT_HANDLER_H
#include <wangle/channel/Handler.h>
#include "common_def.h"
#include "data_manager/roleinfo_manager.h"
#include "sync_redis_wrapper.h"

namespace littleB {
class RoleInjectHandler : public wangle::Handler<CmdMessagePair, ServiceTuple, CmdMessagePair, CmdMessagePair> {
public:
    RoleInjectHandler(RoleinfoManager& roleManager, SyncRedisWrapper& redisWrapper)
        : role_manager_(roleManager), redis_wrapper_(redisWrapper) {}
    /**
     * 当 role 没有从数据库中取到时，填充一个空的 role，来保证 Login 操作依然可以进行
     */
    void read(Context* ctx, CmdMessagePair msg) override;

    // TODO 看一下 wangle 的 proxy example， 看一下 inactive 和 readEOF 的区别
    void transportInactive(Context* ctx) override;

    /* 仅透传 */
    folly::Future<folly::Unit> write(Context* ctx, CmdMessagePair msg) override { return ctx->fireWrite(std::move(msg)); }

private:
    RoleinfoManager& role_manager_;
    SyncRedisWrapper& redis_wrapper_;
};
}  // namespace littleB
#endif  // LITTLEB_ROLE_INJECT_HANDLER_H
