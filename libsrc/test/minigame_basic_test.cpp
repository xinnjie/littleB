//
// Created by xinnjie on 2019/9/28.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <catch2/catch.hpp>

#include <spdlog/spdlog.h>
#include "login_service/minigame_login_service.h"
#include "login_service/minigame_register_service.h"
#include "sync_redis_wrapper.h"

using namespace littleB;
using namespace std;

TEST_CASE("basic service test", "[minigame basic test]") {
    spdlog::set_level(spdlog::level::trace);  // Set global log level to trace
    spdlog::set_pattern("[%H:%M:%S %z] [%l] [thread %t] [%s:%#] [%!] %v");
    spdlog::flush_on(spdlog::level::trace);

    SyncRedisWrapper redis_wrapper;
    redis_wrapper.Connect("127.0.0.1", 6379, timeval{1, 500000});
    RoleInfo empty_role;

    SECTION("login service test") {
        int rpc_id = 111;
        string username = "hello";
        string password = "world";

        MinigameFakeLoginService login_service(redis_wrapper);
        LoginReq req;
        req.set_rpcid(rpc_id);
        req.set_account(username);
        req.set_password(password);

        auto rsp = login_service(empty_role, req);

        REQUIRE(rsp.rpcid() == rpc_id);
        REQUIRE(rsp.error() == 0);
    }

    SECTION("register already exist account") {
        int rpc_id = 222;
        string username = "hello";
        string password = "no use password";
        MinigameRegisterService register_service(redis_wrapper);
        RegisterReq req;
        req.set_rpcid(rpc_id);
        req.set_account(username);
        req.set_password(password);

        auto rsp = register_service(empty_role, req);
        REQUIRE(rsp.rpcid() == rpc_id);
        REQUIRE(rsp.error() == RegisterRsp::ACCOUNT_ALREADY_EXIST);
    }

    SECTION("register new account") {
        int rpc_id = 333;
        string username = "test_account";
        string password = "test_password";
        SECTION("clean existing account") {
            redis_wrapper.RedisCommand("del __password_%s", username.c_str());
            redis_wrapper.RedisCommand("del __role_%s", username.c_str());

            SECTION("now do registeration") {
                MinigameRegisterService register_service(redis_wrapper);
                RegisterReq req;
                req.set_rpcid(rpc_id);
                req.set_account(username);
                req.set_password(password);

                auto rsp = register_service(empty_role, req);
                REQUIRE(rsp.rpcid() == rpc_id);
                REQUIRE(rsp.error() == RegisterRsp::SUCCEED);
            }
        }
    }
}
