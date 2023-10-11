#include "RuleNode.h"

void 
RuleNode::addChildNode(std::unique_ptr<RuleNode> node) {
    m_children.push_back(std::move(node));
}

void 
RuleNode::executeRule() {
    if (m_rule) {
        m_rule->execute();
    }
}