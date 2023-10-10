#include "Rule.h"

MessageRule::MessageRule(ts::Node node, const std::string& source) : IRule(node, source) {
    std::cout << "MessageRule created" << std::endl;
}

std::optional<ts::Node> MessageRule::execute() {
    std::cout << "Executing MessageRule..." << std::endl;

    ts::Node expression = node.getNamedChild(1);
    std::cout << expression.getSourceRange(source) << std::endl;

    return std::nullopt;
}