#include "RuleInterpreter.h"
#include <iostream>
#include <string.h>

IRule* RuleInterpreter::createRule(const ts::Node& node, const std::string& source) {
    return RuleMap::getRule(node.getType());
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

void RuleInterpreter::interpretRules(const ts::Node& rulesHead, const std::string& source) {
    RuleNode* root = RuleInterpreter::convertNodeTreeToRuleTree(rulesHead, source);
    RuleInterpreter::executeRuleTree(root, source);
    delete root;
}

void RuleInterpreter::executeRuleTree(RuleNode* root, const std::string& source) {
  if (!root) { 
    return;
  }
  auto rule{root->getRule()};
  if (rule) {
    rule->execute();
  }

  auto children{root->getChildren()};
  for (auto itr{children.begin()}; itr != children.end(); ++itr) {
    RuleInterpreter::executeRuleTree(*itr, source);
  }
}