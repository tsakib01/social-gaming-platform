#include "RuleInterpreter.h"
#include <iostream>
#include <string.h>

IRule* RuleInterpreter::createRule(const ts::Node& node, const std::string& source) {
    // TO-DO: handle other types
    if (node.getType() == "for") {
      return new ForLoopRule();
    }
    return nullptr;
}

RuleNode* RuleInterpreter::convertNodeTreeToRuleTree(const ts::Node& root, const std::string& source) {
    if (root.isNull()) {
        return nullptr;
    }
    RuleNode* ruleNode = new RuleNode(RuleInterpreter::createRule(root, source));
    // Recursively add children to rule tree
    for (uint32_t idx = 0; idx < root.getNumNamedChildren(); ++idx) {
        ts::Node child = root.getNamedChild(idx);
        RuleNode* childRuleNode = convertNodeTreeToRuleTree(child, source);
        ruleNode->addChildNode(childRuleNode);
    }
    return ruleNode;
}

void RuleInterpreter::interpretRules(const ts::Node& rulesHead, std::string source) {
    RuleNode* root = RuleInterpreter::convertNodeTreeToRuleTree(rulesHead, source);
    // To-do: create exection (Post-order traversal)
    delete root;
}