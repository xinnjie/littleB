cmake_minimum_required(VERSION 3.10)
project(littleB)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(cmake_debug)
find_package(protobuf REQUIRED)
find_package(boost REQUIRED)
add_subdirectory(third_party EXCLUDE_FROM_ALL)
add_subdirectory(lib)
add_subdirectory(app)

enable_testing()