#include "Rule.h"

std::optional<std::shared_ptr<RuleNode>> 
InputRule::execute() {
    std::cout << "Executing Parralel ForRule..." << std::endl;
    return std::nullopt;
}