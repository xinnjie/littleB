//
// Created by xinnjie on 2019/10/5.
//

#ifndef LITTLEB_ERASE_ROLE_SERVICE_H
#define LITTLEB_ERASE_ROLE_SERVICE_H

#include "service_interface.h"
#include "data_manager/roleinfo_manager.h"
#include "erase_role.pb.h"

class EraseRoleService : public littleB::SyncServiceInterface<EraseRoleReq, EraseRoleRsp> {
public:
    explicit EraseRoleService(littleB::RoleinfoManager &roleManager) : role_manager_(roleManager) {}

    EraseRoleRsp operator()(RoleInfo &role, const EraseRoleReq &request) override;

private:
    littleB::RoleinfoManager &role_manager_;
};

#endif //LITTLEB_ERASE_ROLE_SERVICE_H
