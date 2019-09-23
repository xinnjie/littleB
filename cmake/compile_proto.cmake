# command:: compile_proto
#
#   Add custom commands to process `.proto` files to C++::
#
#     compile_proto (<OUT_SRCS> <OUT_HDRS>
#                   SOURCE
#                       ${PROTO_FILES}
#                   INCLUDE_PATHS
#                       ${PROTO_INCLUDES})
#
#   `OUT_SRCS`
#     Variable to define with autogenerated source files
#   `OUT_HDRS`
#     Variable to define with autogenerated header files
#   `SOURCE`
#     `.proto` files
#    `PROTOC_CMD`
#       protoc可执行文件的地址
#       xinnjie:  protoc的一个有点自作聪明的feature,需要人工预估一下最后生成的.pb.cc/h文件的位置。
#               生成的.h/.cpp的位置和include 的地址有关
#               问题详见 https://github.com/protocolbuffers/protobuf/issues/1743，
#               要解决这个问题，需要提取公共地址，但是cmake写这个真的麻烦，所以有需要的话直接人工指定地址吧
#               这个特性更垃圾的一点是它只看第一个 --proto_path，气死个人
set(THIS_DIR ${CMAKE_CURRENT_LIST_DIR})

function(get_canonical_path OUT_CANONICAL_PATH proto_path include_path)
    execute_process(COMMAND python ${THIS_DIR}/proto_get_canonical_path.py ${proto_path} ${include_path}
            OUTPUT_VARIABLE CANONICAL_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE) # strip whitrspace
    set(${OUT_CANONICAL_PATH} ${CANONICAL_PATH} PARENT_SCOPE)
endfunction()

function(compile_protobuf OUT_SRCS OUT_HDRS)
    cmake_parse_arguments(
            proto                              # 前缀
            ""
            "CPP_OUT_DIR"         # 一个值的参数
            "SOURCE;INCLUDE_PATHS"         #
            ${ARGN})

    if (NOT PROTOBUF_PROTOC_EXECUTABLE)
        message(FATAL_ERROR "need protoc executable, please set PROTOBUF_PROTOC_EXECUTABLE")
    endif()
    #    message("include_path ${proto_INCLUDE_PATHS}")
    if(proto_INCLUDE_PATHS)
        foreach(include_path ${proto_INCLUDE_PATHS})
            list(APPEND _proto_include_path  "--proto_path=${include_path}")
        endforeach()
    endif()

    list(GET proto_INCLUDE_PATHS 0 FIRST_INCLUDE)

    foreach(proto_file ${proto_SOURCE})
        get_canonical_path(proto_canonical ${proto_file} ${FIRST_INCLUDE})
        string(REPLACE .proto .pb.h gen_hdr ${proto_canonical})
        string(REPLACE .proto .pb.cc gen_src ${proto_canonical})
        set(gen_src ${proto_CPP_OUT_DIR}/${gen_src})
        set(gen_hdr ${proto_CPP_OUT_DIR}/${gen_hdr})

#        message("proto_canonical=${proto_canonical}|proto_file=${proto_file}|FIRST_INCLUDE=${FIRST_INCLUDE}")
#        message("proto_file=${proto_file}|generate gen_src=${gen_src}|gen_hdr=${gen_hdr}")
#        message("Protobuf_PROTOC_EXECUTABLE ${Protobuf_PROTOC_EXECUTABLE}")
        list(APPEND HDRS ${gen_hdr})
        list(APPEND SRCS ${gen_src})
        add_custom_command(
                OUTPUT ${gen_hdr} ${gen_src}
                COMMAND ${Protobuf_PROTOC_EXECUTABLE}
                ARGS ${_proto_include_path} --cpp_out=${proto_CPP_OUT_DIR} ${proto_file}
                DEPENDS ${proto_file}
                COMMENT "===========================gen pb files of ${proto_file}====================="
        )
    endforeach()
    set(${OUT_HDRS} ${HDRS} PARENT_SCOPE)
    set(${OUT_SRCS} ${SRCS} PARENT_SCOPE)
endfunction()
