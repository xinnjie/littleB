# CMake build : library tests

add_executable(example_test
        example_test)
target_link_libraries(example_test
        Catch2::Catch2)

compile_protobuf(CMD_REGISTER_PROTO_SCRS CMD_REGISTER_PROTO_HDRS
        SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/example_service.proto
        INCLUDE_PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/proto
        CPP_OUT_DIR
        ${CMAKE_CURRENT_BINARY_DIR})
add_executable(cmd_regster_test
        cmd_register_test.cpp
        services/example_sync_service.cpp
        services/example_async_service.cpp
        ${CMD_REGISTER_PROTO_SCRS})
target_link_libraries(cmd_regster_test
        PUBLIC
        Catch2::Catch2
        littleb::littleb
        )
target_include_directories(cmd_regster_test
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        )


add_executable(log_test
        log_test.cpp
        )
target_link_libraries(log_test
        PUBLIC
        Catch2::Catch2
        littleb::littleb
        spdlog::spdlog
        )

enable_testing()
#add_test(test1 example_test)
