#ifndef RULE__NODE__H
#define RULE__NODE__H

#include <memory>
#include <algorithm>
#include "Rule.h"

class RuleNode {
public:
    RuleNode(std::shared_ptr<Rule> rule) 
        : m_rule(std::move(rule)) {}

    void addChildNode(std::shared_ptr<RuleNode> node);

    void executeRule(); 

    std::vector<std::shared_ptr<RuleNode>>& getChildren() { return m_children; }

private:
    std::shared_ptr<Rule> m_rule;
    std::vector<std::shared_ptr<RuleNode>> m_children{};
};

#endif