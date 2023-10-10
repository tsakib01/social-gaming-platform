#include "RuleInterpreter.h"
#include <iostream>
#include <string>
#include <algorithm>

std::unique_ptr<IRule> 
RuleInterpreter::createRule(const ts::Node& node, const std::string& source) {
    auto type = node.getType();
    if (type == "match") {
        return std::make_unique<MatchRule>(node);
    } else if (type == "for") {
        return std::make_unique<ForLoopRule>(node);
    }
    return nullptr;
}


std::unique_ptr<RuleNode> 
RuleInterpreter::convertNodeTreeToRuleTree(const ts::Node& root, const std::string& source) {
    if (root.isNull()) {
        return nullptr;
    }

    auto ruleNode = std::make_unique<RuleNode>(createRule(root, source));
    // Recursively add children to rule tree
    for (uint32_t idx = 0; idx < root.getNumNamedChildren(); ++idx) {
        ts::Node child = root.getNamedChild(idx);
        auto childRuleNode = convertNodeTreeToRuleTree(child, source);
        ruleNode->addChildNode(std::move(childRuleNode));
    }
    return ruleNode;
}

// TODO: Move the execution of the rules out of RuleInterpreter
std::unique_ptr<RuleNode> 
RuleInterpreter::interpretRules(const ts::Node& rulesHead, const std::string& source) {
    auto root = convertNodeTreeToRuleTree(rulesHead, source);
    RuleInterpreter::executeRuleTree(root.get());
    return root;
}

// TODO: Move the execution of the rules out of RuleInterpreter
void 
RuleInterpreter::executeRuleTree(RuleNode* root) {
  if (!root) {
    return;
  }
  
  root->executeRule();

  auto& children = root->getChildren();

  for (auto itr{children.begin()}; itr != children.end(); ++itr) {
    RuleInterpreter::executeRuleTree(itr->get());
  }
}