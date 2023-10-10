#ifndef RULE_INTERPRETER_H
#define RULE_INTERPRETER_H

#include <cpp-tree-sitter.h>
#include "Rule.h"

class RuleInterpreter {
public:
    static IRule* createRule(std::optional<ts::Node> node, const std::string& source);
};

#endif