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
#add_executable(example_rpc
#        src/example_rpc.cpp
#        #        ${PROTO}_SCRS
#        )
#target_link_libraries(example_rpc
#        folly
#        protobuf::libprotobuf
#        wangle)


add_executable(redis_client_test
        test/redis_client_test.cpp)
target_link_libraries(redis_client_test
        hiredis)


#print_target_properties(folly)
#print_target_properties(littleb_lib)
#print_target_properties(example_excutable)
#print_target_properties(hiredis)
#print_target_properties(redis_client_test)