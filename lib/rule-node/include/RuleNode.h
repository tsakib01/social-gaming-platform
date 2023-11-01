#ifndef RULE__NODE__H
#define RULE__NODE__H

#include <memory>
#include <algorithm>
#include <optional>
#include <string_view>
#include "Rule.h"

class Rule;

class RuleNode {
public:
    RuleNode() { }

    void setRule(std::unique_ptr<Rule> rule, std::string_view type);

    void addChildNode(std::shared_ptr<RuleNode> node);

    std::optional<std::shared_ptr<RuleNode>> executeRule(); 

    [[nodiscard]] std::vector<std::shared_ptr<RuleNode>>& 
    getChildren() { return m_children; }

    std::shared_ptr<RuleNode>
    getChildrenAtIndex(size_t idx) { return m_children.at(idx); }

    size_t getChildrenSize();

    void resetExecution();

    void deleteReferences();
    
private:
    std::unique_ptr<Rule> m_rule;
    std::vector<std::shared_ptr<RuleNode>> m_children{};
    std::string_view m_type; // for debugging
};

#endif