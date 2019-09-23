//
// Created by xinnjie on 2019/9/23.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include "command_register.h"
#include "example_service.pb.h"
#include "services/example_sync_service.h"

using namespace littleB;

void example_register() {

}
TEST_CASE( "do register", "[register_manager]" ) {
    CommandManager register_manager;
    REQUIRE(RegisterCommand<ExampleSyncService>(register_manager, 1));
    ExampleReq req;
    req.set_example_int(10);
    RoleInfo role{1};
    auto rsp_ptr = boost::dynamic_pointer_cast<ExampleResp>(register_manager(1, role, req));
    REQUIRE(rsp_ptr);

    REQUIRE(rsp_ptr->ret().ret_code() == 10);
    REQUIRE(rsp_ptr->ret().ret_msg() == "this is example service!");
}