#include "Rule.h"

BaseRule::BaseRule(ts::Node node, const std::string_view& source) : Rule(node, source) {
    std::cout << "Base Rule created" << std::endl;
}

std::optional<ts::Node> BaseRule::execute() {
    std::cout << "Executing BaseRule..." << std::endl;

    if (!executed) {
        executed = true;
        ts::Node nextNode = node.getNamedChild(0);
        return nextNode;
    }

    return std::nullopt;
};