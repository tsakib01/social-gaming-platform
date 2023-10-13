#include "RuleNode.h"

void 
RuleNode::addChildNode(std::shared_ptr<RuleNode> node) {
    m_children.push_back(node);
}

void 
RuleNode::executeRule() {
    if (m_rule) {
        m_rule->execute();
    }
}