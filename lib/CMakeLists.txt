# CMake build : library
cmake_minimum_required(VERSION 3.10)

project(LITTLEB
        LANGUAGES CXX
        VERSION 0.1.0
        )
option(BUILD_SHARED_LIBS "Build shared library" OFF) # TODO

protobuf_generate_cpp(PROTO_SCRS PROTO_HDRS example_login.proto)
add_library(littleb STATIC
        ${PROTO_SCRS}
        command_distributor.cpp command_distributor.h game_service.cpp game_service.h service_interface.h role_info.h command_register.cpp command_register.h)
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
        Boost::boost)
target_compile_options(littleb
        PRIVATE
        -Wall -Wextra)
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

