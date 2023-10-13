add_library(gameConfigLoader 
    src/GameConfigLoader.cpp
    src/GameRules.cpp
    src/GameState.cpp
)

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
  ruleNode
  ruleInterpreter
  rule
  expression
  constantManager
)

add_library(constantManager 
    src/ConstantManager.cpp
)

target_include_directories(constantManager
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(constantManager
  tree-sitter-socialgaming
  cpp-tree-sitter
)

install(TARGETS gameConfigLoader constantManager
  ARCHIVE DESTINATION lib
)