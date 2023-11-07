if (BUILD_TESTS STREQUAL OFF)
    return()
endif()

CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG release-1.12.1
)

enable_testing()

file(
  COPY "${CMAKE_CURRENT_SOURCE_DIR}/games"
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(runAllTests 
    expressionTests.cpp
    userManagerTests.cpp
    ruleFactoryTests.cpp
)

target_link_libraries(runAllTests
    GTest::gtest_main
    gmock
    expression
    userManager
    gameConfigLoader
)