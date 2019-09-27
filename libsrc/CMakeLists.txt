# CMake build : library
cmake_minimum_required(VERSION 3.10)

project(LITTLEB
        LANGUAGES CXX
        VERSION 0.1.0
        )

compile_protobuf(PROTO_SCRS PROTO_HDRS
        SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/role.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/login.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/cmd_id.proto
        INCLUDE_PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/proto
        CPP_OUT_DIR
        ${CMAKE_CURRENT_BINARY_DIR})

add_library(littleb STATIC
        ${PROTO_SCRS}
        handlers/command_dispatcher.cpp
        handlers/command_dispatcher.h
        service_interface.h
        data_manager/command_manager.cpp
        data_manager/command_manager.h
        handlers/cmd_message_serialize_handler.cpp
        handlers/cmd_message_serialize_handler.h
        register_helper.h
        data_manager/pb_reflection_manager.cpp
        data_manager/pb_reflection_manager.h
        common_def.h
        handlers/role_inject_handler.cpp
        handlers/role_inject_handler.h
        data_manager/roleinfo_manager.cpp
        data_manager/roleinfo_manager.h
        handlers/opcode_inject_handler.cpp
        handlers/opcode_inject_handler.h
        sync_redis_wrapper.cpp
        sync_redis_wrapper.h
        login_service/minigame_login_service.cpp
        login_service/minigame_login_service.h
        handlers/LengthFieldBasedFrameDecoder.cpp
        handlers/LengthFieldPrepender.cpp
        )
target_include_directories(littleb
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_INSTALL_INCLUDEDIR}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
target_link_libraries(littleb
        PUBLIC
        OpenSSL::SSL  # folly&wangle has openssl depencdency, but not link
        Threads::Threads # folly dependency
        Folly::folly
        protobuf::libprotobuf
        wangle::wangle
        Boost::boost
        spdlog::spdlog
        hiredis
        )
target_compile_options(littleb
        PRIVATE
        -Wall -Wextra -Wno-unused )
set_target_properties(littleb
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
        )

add_library(littleb::littleb ALIAS littleb)
# test
enable_testing()
add_subdirectory(test)

