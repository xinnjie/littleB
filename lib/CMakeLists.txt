# CMake build : library
cmake_minimum_required(VERSION 3.10)

project(LITTLEB
        LANGUAGES CXX
        VERSION 0.1.0
        )
option(BUILD_SHARED_LIBS "Build shared library" OFF) # TODO

protobuf_generate_cpp(PROTO_SCRS PROTO_HDRS example_login.proto)
add_library(littleb STATIC
        ${PROTO_SCRS}
        command_distributor.cpp command_distributor.h game_service.cpp game_service.h service_interface.cpp include/service_interface.h include/example_include.h)
target_include_directories(littleb
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
target_link_libraries(littleb
        PUBLIC
        folly
        protobuf::libprotobuf
        wangle
        Boost::boost)
target_compile_options(littleb
        PRIVATE
        -Wall -Wextra)
set_target_properties(littleb
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
        )

add_library(littleb::littleb ALIAS littleb)
# test
enable_testing()
add_subdirectory(test)


# install headers & libs
include(GenerateExportHeader)
generate_export_header(littleb
        EXPORT_MACRO_NAME LITTLEB_API
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/common.h
        )
include(GNUInstallDirs)
install(TARGETS littleb
        EXPORT littleb-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/littleb
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/littleb
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/littleb
        )
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
#        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/littleb
#        )
# generate littleb-targets.cmake
install(EXPORT littleb-targets
        FILE littleb-targets.cmake
        NAMESPACE littleb::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/littleb
        )

# generate littleb-config.cmake & littleb-config-version.cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/littleb-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/littleb-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/littleb
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/littleb-config-version.cmake
        VERSION ${LITTLEB_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/littleb-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/littleb-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/littleb
        )
#        There are three ways to access a project from another project: subdirectory, exported build directories, and installing
#        quoted from  https://cliutils.gitlab.io/modern-cmake/chapters/install/exporting.html
#        personally 1st is my favorite, 3rd is ok, and 2nd is not necessary, so I drop 2nd method
#        export(EXPORT littleb-targets
#        FILE ${CMAKE_BINARY_DIR}/cmake/littleb-targets.cmake
#        NAMESPACE littleb::
#        )