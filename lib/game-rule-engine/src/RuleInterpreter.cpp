#include "RuleInterpreter.h"
#include <iostream>
#include <string.h>

RuleNode* RuleInterpreter::convertNodeTreeToRuleTree(const ts::Node& root, const std::string& source) {
    if (root.isNull()) {
        return nullptr;
    }
    RuleNode* ruleNode = new RuleNode(RuleMap::getRule(root.getType(), root));
    // Recursively add children to rule tree
    for (uint32_t idx = 0; idx < root.getNumNamedChildren(); ++idx) {
        ts::Node child = root.getNamedChild(idx);
        RuleNode* childRuleNode = convertNodeTreeToRuleTree(child, source);
        ruleNode->addChildNode(childRuleNode);
    }
    return ruleNode;
}

void RuleInterpreter::interpretRules(const ts::Node& rulesHead, const std::string& source) {
    RuleNode* root = convertNodeTreeToRuleTree(rulesHead, source);
    RuleInterpreter::executeRuleTree(root, source);
    delete root;
}

void RuleInterpreter::executeRuleTree(RuleNode* root, const std::string& source) {
  if (!root) {
    return;
  }
  
  root->executeRule();

  auto children{root->getChildren()};

  for (auto itr{children.begin()}; itr != children.end(); ++itr) {
    RuleInterpreter::executeRuleTree(*itr, source);
  }
}