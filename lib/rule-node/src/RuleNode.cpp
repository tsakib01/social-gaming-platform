#include "RuleNode.h"

void
RuleNode::setRule(std::unique_ptr<Rule> rule, std::string_view type) {
    m_rule = std::move(rule);
    m_type = type;
}

void 
RuleNode::addChildNode(std::shared_ptr<RuleNode> node) {
    m_children.push_back(node);
}

std::optional<std::shared_ptr<RuleNode>>
RuleNode::executeRule() {
    if (m_rule) {
        return m_rule->execute();
    }
    return std::nullopt;
}

size_t
RuleNode::getChildrenSize() {
    return m_children.size();
}

void RuleNode::resetExecution() {
    if (m_rule) {
        m_rule->reset();
    }
}

void RuleNode::deleteReferences() {
    for (std::shared_ptr child : m_children) {
        child->deleteReferences();
    }
    m_rule.reset();
}
