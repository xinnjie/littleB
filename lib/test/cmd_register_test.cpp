//
// Created by xinnjie on 2019/9/23.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include "command_manager.h"
#include "pb_reflection_manager.h"
#include  "register_helper.h"
#include "example_service.pb.h"
#include "services/example_async_service.h"
#include "services/example_sync_service.h"

using namespace littleB;

TEST_CASE("do sync register", "[sync service]") {
    CommandManager register_manager;
    PbReflectionManager reflection_manager;
    REQUIRE(RegisterSyncCommand<ExampleSyncService>(register_manager, reflection_manager, 1));
    ExampleReq req;
    req.set_example_int(10);
    RoleInfo role{1};
    auto rsp_ptr = boost::dynamic_pointer_cast<ExampleResp>(register_manager.RunSyncService(1, role, req));
    REQUIRE(rsp_ptr);

    REQUIRE(rsp_ptr->ret().ret_code() == 10);
    REQUIRE(rsp_ptr->ret().ret_msg() == "this is example service!");
}

TEST_CASE("do async register", "[async service]") {
    CommandManager register_manager;
    PbReflectionManager reflection_manager;
    REQUIRE(RegisterAsyncCommand<ExampleAsyncService>(register_manager, reflection_manager, 1));
    ExampleReq req;
    req.set_example_int(10);
    RoleInfo role{1};
    auto rsp_future_ptr = (register_manager.RunAsyncService(1, role, req));
    auto rsp_ptr = boost::dynamic_pointer_cast<ExampleResp>(std::move(rsp_future_ptr.value()));
    REQUIRE(rsp_ptr);

     REQUIRE(rsp_ptr->ret().ret_code() == 10);
    REQUIRE(rsp_ptr->ret().ret_msg() == "succeed");
}