#ifndef RULE_INTERPRETER_H
#define RULE_INTERPRETER_H

#include <memory>
#include <cpp-tree-sitter.h>
#include "Rule.h"
#include "RuleNode.h"

class RuleInterpreter {
public:
    static std::unique_ptr<IRule> createRule(const ts::Node& node, const std::string& source);
    static std::unique_ptr<RuleNode> convertNodeTreeToRuleTree(const ts::Node& root, const std::string& source);
    static std::unique_ptr<RuleNode> interpretRules(const ts::Node& rulesHead, const std::string& source);
    static void executeRuleTree(RuleNode* root);
};

#endif