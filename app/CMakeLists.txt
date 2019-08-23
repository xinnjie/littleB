# CMake build : main application

add_executable(example_excutable
        src/main.cpp)

target_link_libraries(example_excutable
        folly)