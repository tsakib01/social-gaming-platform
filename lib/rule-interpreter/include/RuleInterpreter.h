#ifndef RULE_INTERPRETER_H
#define RULE_INTERPRETER_H

#include <memory>
#include <cpp-tree-sitter.h>
#include "Rule.h"
#include "RuleNode.h"

class RuleInterpreter {
public:
    static std::shared_ptr<Rule> createRule(std::optional<ts::Node> node, std::string_view source);

    // Note: May be used in future iterations rather than using ts::Node directly
    // Converts a raw ts::Node into a RuleNode wrapper object
    static std::unique_ptr<RuleNode> convertNodeTreeToRuleTree(const ts::Node& root, std::string_view source);
};

#endif