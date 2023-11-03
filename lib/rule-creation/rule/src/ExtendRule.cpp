#include "Rule.h"

std::optional<std::shared_ptr<RuleNode>> 
ExtendRule::execute() {
    std::cout << "Executing ExtendRule..." << std::endl;
    return std::nullopt;
}