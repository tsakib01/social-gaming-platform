#ifndef RULE__H
#define RULE__H

#include <vector>
#include <cpp-tree-sitter.h>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <string_view>

class IRule {
public:
    virtual void execute() = 0;
    virtual ~IRule() {}
};

class ForLoopRule : public IRule {
public:
    ForLoopRule(const ts::Node node) : m_node(node) {
        std::cout << "For Loop Rule created" << std::endl;
    }

    void execute() override;
private:
    const ts::Node m_node;
};

class MatchRule : public IRule {
public:
    MatchRule(const ts::Node node) : m_node(node) {
        std::cout << "Match Rule created" << std::endl;
    }

    void execute() override;
private:
    const ts::Node m_node;
};

// This allows a cleaner way to map from a string representing a rule in the 
// treesitter language to their corresponding Rule constructor
class RuleMap {
public:
    using MapType = std::unordered_map<std::string_view, std::function<IRule*(const ts::Node&)>>; 

    // Takes in a string representing a rule, and returns the appropriate Rule object
    static IRule* getRule(std::string_view ruleName, const ts::Node& nodeRef);

private:
    template <typename RuleType>
    static IRule* createRule(const ts::Node& node);

    // Static initializer for the m_map static variable
    static MapType generate();
  
    static inline MapType m_map{ generate() };
};

#endif