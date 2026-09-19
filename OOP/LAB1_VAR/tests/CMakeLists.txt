find_package(GTest REQUIRED)

add_executable(test_variant
    test_variant.cpp                 
    ../src/matrix_ops.cpp
)

target_include_directories(test_variant PRIVATE ../src)
target_link_libraries(test_variant PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_variant)