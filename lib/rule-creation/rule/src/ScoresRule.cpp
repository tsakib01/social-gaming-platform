#include "Rule.h"

std::optional<std::shared_ptr<RuleNode>> 
ScoresRule::execute() {
    std::cout << "Executing Scores Rule..." << std::endl;
    return std::nullopt;
}