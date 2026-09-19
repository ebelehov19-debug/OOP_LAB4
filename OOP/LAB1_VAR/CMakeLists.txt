cmake_minimum_required(VERSION 3.16)
project(Lab01_Variant15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab01
    src/main.cpp
    src/matrix_ops.cpp
)

target_include_directories(lab01 PRIVATE src)

enable_testing()
add_subdirectory(tests)