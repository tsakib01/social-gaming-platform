#ifndef CONSTANT_MANAGER
#define CONSTANT_MANAGER

#include <cpp-tree-sitter.h>
#include <map>
#include <string>
#include <vector>
#include<string_view>

// This file is a temporary class while statemanager is developed.
// Write now, I am only asssuming the mapping from name -> list of strings
class ConstantManager{
public:
    ConstantManager(const ts::Node & constantRoot, std::string_view source);
    void print();
    void printByLevelOrder(const ts::Node& node);
    int getNumberByName(std::string_view name);
private:
    // Member vairabls
    std::map<std::string_view, std::vector<std::string_view>> constantListOfString;
    std::map<std::string_view, int> constantListOfNumber;
    std::string_view source;

    void interpretConstant(const ts::Node& constantRoot);
    std::vector<std::string_view> getStringList(const ts::Node& expressionListNode);
    std::string_view convertQuotedStringToStringView(ts::Node node);
};
#endif