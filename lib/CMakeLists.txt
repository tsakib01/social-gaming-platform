CPMAddPackage(
  NAME cpp-tree-sitter
  GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
  GIT_TAG v0.0.2
)
add_grammar_from_repo(tree-sitter-socialgaming
  https://github.com/nsumner/tree-sitter-socialgaming
  0.0.1
)

add_subdirectory(expression)
add_subdirectory(game-config-loader)
add_subdirectory(game-instance-manager)
add_subdirectory(game-instance)
add_subdirectory(networking)
add_subdirectory(rule)
add_subdirectory(rule-interpreter)
add_subdirectory(rule-node)
