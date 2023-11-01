#include "Rule.h"

BodyRule::BodyRule(std::shared_ptr<RuleNode> node, std::string_view source) : Rule(node, source) {
    std::cout << "Body Rule created" << std::endl;
}

std::optional<std::shared_ptr<RuleNode>>
BodyRule::execute() {
    // std::cout << "Executing BodyRule..." << std::endl;

    if (index < node->getChildrenSize()) {
        std::shared_ptr<RuleNode> nextNode = std::move(node->getChildrenAtIndex(index));
        index++;
        return nextNode;
    } 

    return std::nullopt;
}

void BodyRule::reset() {
    index = 0;
    for (std::shared_ptr<RuleNode> child : node->getChildren()) {
        child->resetExecution();
    }
}