cmake_minimum_required(VERSION 3.10)
project(gamesvr)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_subdirectory(third_party/gflags)
add_subdirectory(third_party/folly)


add_executable(gamesvr main.cpp)
target_link_libraries(gamesvr
        folly)