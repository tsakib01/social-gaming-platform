#include "RuleInterpreter.h"
#include <iostream>
#include <string.h>

IRule* RuleInterpreter::createRule(const ts::Node& node, const std::string& source) {
    if (node.getType() == "for") {
      return new ForLoopRule(node);
    }
    return nullptr;
}


RuleNode* RuleInterpreter::convertNodeTreeToRuleTree(const ts::Node& root, const std::string& source) {
    if (root.isNull()) {
        return nullptr;
    }
    RuleNode* ruleNode = new RuleNode(createRule(root, source));
    // Recursively add children to rule tree
    for (uint32_t idx = 0; idx < root.getNumNamedChildren(); ++idx) {
        ts::Node child = root.getNamedChild(idx);
        RuleNode* childRuleNode = convertNodeTreeToRuleTree(child, source);
        ruleNode->addChildNode(childRuleNode);
    }
    return ruleNode;
}

// TODO: Move the execution of the rules out of RuleInterpreter
void RuleInterpreter::interpretRules(const ts::Node& rulesHead, const std::string& source) {
    RuleNode* root = convertNodeTreeToRuleTree(rulesHead, source);
    RuleInterpreter::executeRuleTree(root, source);
    delete root;
}

// TODO: Move the execution of the rules out of RuleInterpreter
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