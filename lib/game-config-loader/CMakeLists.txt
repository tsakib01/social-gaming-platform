CPMAddPackage(
  NAME cpp-tree-sitter
  GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
  GIT_TAG v0.0.2
)
CPMAddPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  VERSION 3.9.1
)

add_grammar_from_repo(tree-sitter-socialgaming
  https://github.com/nsumner/tree-sitter-socialgaming
  0.0.1
)

add_library(gameState src/GameState.cpp)
target_include_directories(gameState
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(gameState
  nlohmann_json::nlohmann_json
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
  tree-sitter-socialgaming
  cpp-tree-sitter
  gameRuleEngine
  nlohmann_json
  gameState
)

install(TARGETS gameConfigLoader
  ARCHIVE DESTINATION lib
)