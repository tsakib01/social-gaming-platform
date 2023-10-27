#include "Rule.h"

std::optional<std::shared_ptr<RuleNode>> 
AssignmentRule::execute() {
    std::cout << "Executing AssignmentRule..." << std::endl;
    return std::nullopt;
}