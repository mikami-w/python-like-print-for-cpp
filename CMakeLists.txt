cmake_minimum_required(VERSION 3.20)
project(pyprint)

set(CMAKE_CXX_STANDARD 17)

# Enable ASan for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -g -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

# Test executable
add_executable(test_pyprint tests/test_pyprint.cpp)
