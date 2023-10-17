#include "Rule.h"

std::optional<std::shared_ptr<RuleNode>> 
ParallelForRule::execute() {
    std::cout << "Executing Parallel For Rule..." << std::endl;
    return std::nullopt;
}