# CMake build : main application

add_executable(minigame_server
        minigame_sever_main.cpp
        ${MINIGAME_SRC}
        service/task_progress_service.cpp service/task_progress_service.h)
target_include_directories(minigame_server
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        )
target_link_libraries(minigame_server
        littleb::littleb)

add_executable(folly_test
        test/folly.cpp
        #        ${PROTO}_SCRS
        )
target_link_libraries(folly_test
        littleb::littleb
        Catch2::Catch2)


add_executable(redis_client_test
        test/redis_client_test.cpp)
target_link_libraries(redis_client_test
        hiredis)


#print_target_properties(folly)
#print_target_properties(littleb_lib)
#print_target_properties(example_excutable)
#print_target_properties(hiredis)
#print_target_properties(redis_client_test)