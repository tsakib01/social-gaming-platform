if (BUILD_TESTS STREQUAL OFF)
    return()
endif()

# requires GoogleTest is installed > sudo apt-get install libgtest-dev
find_package(GTest REQUIRED)

enable_testing()

add_executable(runAllTests test.cpp)
target_link_libraries(runAllTests GTest::gtest_main)