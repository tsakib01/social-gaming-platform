#ifndef RULE_INTERPRETER_H
#define RULE_INTERPRETER_H

#include <cpp-tree-sitter.h>
#include "Rule.h"

class RuleInterpreter {
public:
    static std::shared_ptr<Rule> createRule(std::optional<ts::Node> node, const std::string_view& source);
};

#endif