#include "Rule.h"

BaseRule::BaseRule(std::shared_ptr<RuleNode> node, std::string_view source) : Rule(node, source) {
    std::cout << "Base Rule created" << std::endl;
}

std::optional<std::shared_ptr<RuleNode>> 
BaseRule::execute() {
    // std::cout << "Executing BaseRule..." << std::endl;

    if (!executed) {
        executed = true;
        std::shared_ptr<RuleNode> nextNode = node->getChildrenAtIndex(0);
        return nextNode;
    }

    return std::nullopt;
};

void BaseRule::reset() {
    executed = false;
    for (std::shared_ptr<RuleNode> child : node->getChildren()) {
        child->resetExecution();
    }
}