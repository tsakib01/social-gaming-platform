CPMAddPackage(
  NAME cpp-tree-sitter
  GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
  GIT_TAG v0.0.1
)
add_grammar_from_repo(tree-sitter-json
  https://github.com/tree-sitter/tree-sitter-json.git
  0.19.0
)

add_library(gameConfigLoader src/GameConfigLoader.cpp)
target_include_directories(gameConfigLoader
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(gameConfigLoader
  tree-sitter-json
  cpp-tree-sitter
)

install(TARGETS gameConfigLoader
  ARCHIVE DESTINATION lib
)