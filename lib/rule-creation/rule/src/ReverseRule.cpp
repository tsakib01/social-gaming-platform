#include "Rule.h"

std::optional<std::shared_ptr<RuleNode>> 
ReverseRule::execute() {
    std::cout << "Executing ReversedRule..." << std::endl;
    return std::nullopt;
}