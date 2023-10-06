#ifndef RULE_INTERPRETER_H
#define RULE_INTERPRETER_H

#include <cpp-tree-sitter.h>
#include "Rule.h"
#include "RuleNode.h"

class RuleInterpreter {
public:
    static IRule* createRule(const ts::Node& node, const std::string& source);
    static RuleNode* convertNodeTreeToRuleTree(const ts::Node& root, const std::string& source);
    static void interpretRules(const ts::Node& rulesHead, std::string source);
};

#endif