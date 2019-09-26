//
// Created by xinnjie on 2019/9/26.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#include "data_manager/pb_reflection_manager.h"
#include "data_manager/roleinfo_manager.h"
#include "role.pb.h"
#include "service/minigame_login_service.h"
#include "sync_redis_wrapper.h"

using namespace littleB;

TEST_CASE("login service test", "[]") {
    RoleinfoManager role_manager;
    PbReflectionManager reflection_manager;
    SyncRedisWrapper redis_wrapper;
    redis_wrapper.Connect("127.0.0.1", 6379, timeval{1, 500000});
    MinigameLoginService login_service(redis_wrapper);
    C2R_Login req;
    req.set_rpcid(31);
    req.set_account("hello");
    req.set_password("world");
    RoleInfo role;
    R2C_Login rsp = login_service(role, req);
    REQUIRE(rsp.rpcid() == req.rpcid());
}
