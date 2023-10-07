#ifndef CONSTANT_MANAGER
#define CONSTANT_MANAGER

#include <cpp-tree-sitter.h>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include<string_view>
#include <queue>

// This file is a temporary class while statemanager is developed.
// Write now, I am only asssuming the mapping from name -> list of strings
class ConstantManager{
public:
    ConstantManager(const ts::Node & constantRoot, std::string_view source);
    void print();
    void print_by_level_order(const ts::Node& node);
private:
    // Member vairabls
    std::map<std::string_view, std::vector<std::string_view>> constant_list;
    std::string_view source;

    void interpretConstant(const ts::Node& constantRoot);
    std::vector<std::string_view> get_string_list(const ts::Node & expression_list_node);
    std::string_view convert_quoted_string_to_string_view(ts::Node node);
};
#endif