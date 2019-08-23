# CMake build : library tests

add_executable(example_test
        example_test)
target_link_libraries(example_test
        Catch2::Catch2)
enable_testing()

add_test(test1 example_test)