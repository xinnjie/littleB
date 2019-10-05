# CMake build : main application

compile_protobuf(PROTO_SCRS PROTO_HDRS
        SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/task_db.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/task_progress.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/erase_role.proto
        INCLUDE_PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/proto
        CPP_OUT_DIR
        ${CMAKE_CURRENT_BINARY_DIR})


add_executable(minigame_server
        minigame_sever_main.cpp
        ${PROTO_SCRS}
        service/task_query_service.cpp
        service/task_query_service.h
        data_manager/task_data_manager.cpp
        data_manager/task_data_manager.h service/task_update_service.cpp service/task_update_service.h)
target_include_directories(minigame_server
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}

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

# install
INSTALL(TARGETS minigame_server DESTINATION bin)
#print_target_properties(folly)
#print_target_properties(littleb_lib)
#print_target_properties(example_excutable)
#print_target_properties(hiredis)
#print_target_properties(redis_client_test)