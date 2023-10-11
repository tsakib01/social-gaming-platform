#include "RuleInterpreter.h"
#include <iostream>
#include <string.h>

std::shared_ptr<Rule> RuleInterpreter::createRule(std::optional<ts::Node> node, const std::string_view source) {
    if (node.has_value()) {
        ts::Node actualNode = node.value();
        std::string_view type = actualNode.getType();

        if (type == "body") {
            return std::make_shared<BodyRule>(actualNode, source);
        }
        if (type == "rule") {
            return std::make_shared<BaseRule>(actualNode, source);
        }
        if (type == "message") {
            return std::make_shared<MessageRule>(actualNode, source);
        }

        throw std::runtime_error("Rule hasn't been created yet.");
    }

    return nullptr;
}