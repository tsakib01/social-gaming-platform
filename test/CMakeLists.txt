if (BUILD_TESTS STREQUAL OFF)
    return()
endif()

CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG release-1.12.1
)

enable_testing()

add_executable(runAllTests 
    expressionTests.cpp
)

target_link_libraries(runAllTests
    GTest::gtest_main
    expression
)