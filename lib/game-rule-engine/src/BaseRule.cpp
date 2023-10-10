#include "Rule.h"

BaseRule::BaseRule(ts::Node node, const std::string& source) : IRule(node, source) {
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