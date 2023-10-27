#ifndef RULE_INTERPRETER_H
#define RULE_INTERPRETER_H

#include <cpp-tree-sitter.h>
#include <memory>
#include "Rule.h"
#include "RuleNode.h"

class RuleInterpreter {
public:
    [[nodiscard]] static std::shared_ptr<RuleNode> 
    createRuleNode(std::optional<ts::Node> node, std::string_view source);

    // Note: May be used in future iterations rather than using ts::Node directly
    // Converts a raw ts::Node into a RuleNode wrapper object
    [[nodiscard]] static std::shared_ptr<RuleNode> 
    convertNodeTreeToRuleTree(const ts::Node& root, std::string_view source);
};

#endif