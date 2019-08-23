cmake_minimum_required(VERSION 3.10)
project(gamesvr)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_subdirectory(third_party EXCLUDE_FROM_ALL)
add_subdirectory(lib)
add_subdirectory(app)

enable_testing()