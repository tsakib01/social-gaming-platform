# Social Gaming

## cpp-tree-sitter with tree-sitter-socialgaming

# Steps to build:
1) In a seperate build directory, run `cmake ../parser` followed by `make`
2) run `bin/demo`

# Parser Tips:
1) Core API for cpp-tree-sitter can be found in `<build-directory>/_deps/cpp-tree-sitter-src/include/cpp-tree-sitter.h`
2) Using the tree-sitter-grammar discussion post: https://coursys.sfu.ca/2023fa-cmpt-373-d1/forum/91

# Steps to recreate demo:
1) Copy the demo.cpp and CMakeLists snippet from: https://github.com/nsumner/cpp-tree-sitter
2) Add CPM: https://github.com/cpm-cmake/CPM.cmake
3) Copy the rock-paper-scissors.game from: https://github.com/nsumner/tree-sitter-socialgaming/tree/main/games
4) Add basic commands to CMakeLists including:
    * add tree-sitter-socialgaming
    * copying the rock-paper-scissors.game to build directory
5) Change the demo.cpp to read in and parse rock-paper-scissors.game