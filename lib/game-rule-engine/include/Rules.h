#ifndef RULES__H
#define RULES__H

#include "RuleNode.h"
#include <cpp-tree-sitter.h>
#include <string>
#include <memory>

class Rules {
public:
    Rules(std::unique_ptr<RuleNode> rules, std::string_view source)
        : m_rules(std::move(rules)), m_source(source) 
    {}

    RuleNode& getRules() { return *m_rules; }
    std::string_view getSource() { return m_source; }
private:
    std::unique_ptr<RuleNode> m_rules;
    std::string m_source;
};

#endif