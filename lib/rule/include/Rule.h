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
    MatchRule(const ts::Node node, std::string_view source) 
        : m_node(node), m_source(source)
    {
        std::cout << "Match Rule created" << std::endl;
    }

    void execute() override;
private:
    const ts::Node m_node;
    std::string_view m_source;
};

#endif