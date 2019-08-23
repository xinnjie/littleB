# CMake build : library

add_library(example_lib STATIC
       src/example_lib.cpp )

target_include_directories(example_lib
        PUBLIC include)
# test
enable_testing()
add_subdirectory(test)