# CMake build : main application

#protobuf_generate_cpp(PROTO_SCRS PROTO_HDRS src/proto/example_service.proto)
#
#add_executable(example_excutable
#        src/main.cpp
#        ${PROTO_SCRS}
#        )
#
#target_link_libraries(example_excutable
#        PUBLIC
#        Folly::folly
#        wangle::wangle
#        littleb::littleb)
#
#target_include_directories(example_excutable
#        PUBLIC
#        ${CMAKE_CURRENT_BINARY_DIR})
add_executable(example_server
        server_main.cpp
        #        ${PROTO}_SCRS
        )
target_link_libraries(example_server
        littleb::littleb)

#compile_protobuf(PROTO_SCRS PROTO_HDRS
#        SOURCE
#        INCLUDE_PATHS
#        ${CMAKE_CURRENT_SOURCE_DIR}/proto
#        CPP_OUT_DIR
#        ${CMAKE_CURRENT_BINARY_DIR}
#        )
#set(MINIGAME_SRC
#        ${PROTO_SCRS}
#        )

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

add_executable(service_login_test
        test/service_login_test.cpp
        ${MINIGAME_SRC}
        )
target_include_directories(service_login_test
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
target_link_libraries(service_login_test
        littleb::littleb
        Catch2::Catch2)

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