#include "Rule.h"

std::optional<std::shared_ptr<RuleNode>> 
ReverseRule::execute() {
    std::cout << "Executing ReverseRule..." << std::endl;
    return std::nullopt;
}