//
// Created by xinnjie on 2019/9/25.
//

#ifndef LITTLEB_ROLE_INJECT_HANDLER_H
#define LITTLEB_ROLE_INJECT_HANDLER_H
#include <wangle/channel/Handler.h>
#include "common_def.h"
#include "data_manager/roleinfo_manager.h"

namespace littleB {
class RoleInjectHandler : public wangle::InboundHandler<CmdMessagePair, ServiceTuple> {
public:
    explicit RoleInjectHandler(RoleinfoManager& roleManager) : role_manager_(roleManager) {}
    /**
     * 当 role 没有从数据库中取到时，填充一个空的 role，来保证 Login 操作依然可以进行
     */
    void read(Context* ctx, CmdMessagePair msg) override;

private:
    RoleinfoManager& role_manager_;
};
}  // namespace littleB
#endif  // LITTLEB_ROLE_INJECT_HANDLER_H
