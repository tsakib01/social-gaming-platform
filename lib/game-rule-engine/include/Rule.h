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

#endif