cmake_minimum_required(VERSION 3.19)
project(socialgaming)

include(cmake/CPM.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set (CMAKE_CXX_STANDARD 20)

# Downloads this wrapper library and tree-sitter.
# Makes them available via the `cpp-tree-sitter` CMake library target.
CPMAddPackage(
  NAME cpp-tree-sitter
  GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
  GIT_TAG v0.0.1
)

# Downloads a tree-sitter grammar from github and makes it available as a
# cmake library target.
add_grammar_from_repo(tree-sitter-socialgaming                 # Defines the library name for a grammar
  https://github.com/nsumner/tree-sitter-socialgaming          # Repository URL of a tree-sitter grammar
  0.0.1                                                        # Version tag for the grammar
)

file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/rock-paper-scissors.game
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/games/
)

# Use the library in a demo program.
add_executable(demo)
target_sources(demo
  PRIVATE
    demo.cpp
)
target_link_libraries(demo
  tree-sitter-socialgaming
  cpp-tree-sitter
)