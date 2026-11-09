cmake_minimum_required(VERSION 3.20)
project(Lab04_Bank_Variant12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(bank STATIC
    src/Bank_Optional.cpp
    src/BankCollection.cpp
    src/Deposit.cpp
    src/Credit.cpp
    src/Card.cpp
)
target_include_directories(bank PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

function(enable_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()
enable_warnings(bank)

option(ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)
if(ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND NOT MSVC)
        target_compile_options(bank PUBLIC -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(bank PUBLIC -fsanitize=address,undefined)
    else()
        message(FATAL_ERROR "Sanitizers require GCC or Clang with sanitizer support")
    endif()
endif()

include(CTest)
if(BUILD_TESTING)
    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        include(FetchContent)
        if(POLICY CMP0135)
            cmake_policy(SET CMP0135 NEW)
        endif()
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
        FetchContent_Declare(googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
        )
        FetchContent_MakeAvailable(googletest)
    endif()
    add_executable(bank_tests tests/test_bank.cpp tests/test_collection.cpp)
    target_link_libraries(bank_tests PRIVATE bank GTest::gtest_main)
    enable_warnings(bank_tests)
    include(GoogleTest)
    gtest_discover_tests(bank_tests)
endif()
