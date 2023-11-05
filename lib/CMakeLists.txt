CPMAddPackage(
  NAME cpp-tree-sitter
  GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
  GIT_TAG v0.0.2
)
add_grammar_from_repo(tree-sitter-socialgaming
  https://github.com/nsumner/tree-sitter-socialgaming
  0.0.1
)

add_subdirectory(game-creation)
add_subdirectory(rule-creation)
add_subdirectory(message-handler)
add_subdirectory(networking)
add_subdirectory(user-manager)
