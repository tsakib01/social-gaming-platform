#include "Rule.h"

std::optional<std::shared_ptr<RuleNode>> 
ForRule::execute() {
    if (iteration == maxLength) {
        return std::nullopt;
    }
    std::cout << "Executing For Rule... Iteration:" << iteration << std::endl;
    if (index < node->getChildrenSize()) {
        std::shared_ptr<RuleNode> nextNode = std::move(node->getChildrenAtIndex(index));
        if (index == node->getChildrenSize() - 1) {
            index = 0;
            iteration++;
            for (std::shared_ptr<RuleNode> child : node->getChildren()) {
                child->resetExecution();
            }
        }
        return nextNode;
    } 

    return std::nullopt;
}