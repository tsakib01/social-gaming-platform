#ifndef GAME_RULE_ENGINE_H
#define GAME_RULE_ENGINE_H

#include "RuleInterpreter.h"

class GameRuleEngine {
public:
    GameRuleEngine(){};
    ~GameRuleEngine(){};
    void interpretRules(const ts::Node& rulesHead, std::string source);
};

#endif