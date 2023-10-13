cmake_minimum_required(VERSION 3.24)
project(SocialGaming)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fno-rtti")

if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
  set(NETWORKING_EMSCRIPTEN_BUILD ON)
  set(CMAKE_CXX_FLAGS_DEBUG
      "-O0 -g -fno-omit-frame-pointer"
  )
else()
  set(NETWORKING_EMSCRIPTEN_BUILD OFF)
  set(CMAKE_CXX_FLAGS_DEBUG
      "-O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer"
  )
endif()

file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/games/rock-paper-scissors.game
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/games/
)

include(cmake/CPM.cmake)

add_subdirectory(lib)
add_subdirectory(tools)

set(BUILD_TESTS OFF)
add_subdirectory(test)