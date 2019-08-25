# CMake build : main application

add_executable(example_excutable
        src/main.cpp)

target_link_libraries(example_excutable
        folly)


add_executable(redis_client_test
        redis_client_test.cpp )
target_link_libraries(redis_client_test hiredis)