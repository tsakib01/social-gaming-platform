#include "GameRuleEngine.h"
#include <iostream>
#include <string.h>

void GameRuleEngine::interpretRules(const ts::Node& rulesHead, std::string source) {
    RuleNode* root = RuleInterpreter::convertNodeTreeToRuleTree(rulesHead, source);
    // To-do: create exection (Post-order traversal)
    delete root;
}