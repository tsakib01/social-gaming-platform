#include "RuleInterpreter.h"
#include <iostream>
#include <string.h>

IRule* RuleInterpreter::createRule(std::optional<ts::Node> node, const std::string& source) {
    if (node.has_value()) {
        ts::Node actualNode = node.value();
        std::string_view type = actualNode.getType();

        if (type == "body") {
            return new BodyRule(actualNode, source);
        }
        if (type == "rule") {
            return new BaseRule(actualNode, source);
        }
        if (type == "message") {
            return new MessageRule(actualNode, source);
        }

        throw std::runtime_error("Rule hasn't been created yet.");
    }

    return nullptr;
}