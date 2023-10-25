#include "Rule.h"

std::optional<std::shared_ptr<RuleNode>> 
DiscardRule::execute() {
    std::cout << "Executing DiscardRule..." << std::endl;
    return std::nullopt;
}