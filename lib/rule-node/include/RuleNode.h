#ifndef RULE__NODE__H
#define RULE__NODE__H

#include <memory>
#include <algorithm>
#include "Rule.h"

class RuleNode {
public:
    RuleNode(std::unique_ptr<IRule> rule) 
        : m_rule(std::move(rule)) {}

    void addChildNode(std::unique_ptr<RuleNode> node);

    void executeRule(); 

    std::vector<std::unique_ptr<RuleNode>>& getChildren() { return m_children; }

private:
    std::unique_ptr<IRule> m_rule;
    std::vector<std::unique_ptr<RuleNode>> m_children{};
};

#endif