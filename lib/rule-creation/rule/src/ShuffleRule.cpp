#include "Rule.h"

std::optional<std::shared_ptr<RuleNode>> 
ShuffleRule::execute() {
    std::cout << "Executing ShuffleRule..." << std::endl;
    return std::nullopt;
}