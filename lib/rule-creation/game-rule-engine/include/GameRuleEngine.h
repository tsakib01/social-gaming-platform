#ifndef GAME_RULE_ENGINE_H
#define GAME_RULE_ENGINE_H

#include "RuleInterpreter.h"

class GameRuleEngine {
public:
    static GameRuleEngine& getInstance();
private:
    GameRuleEngine() {}; // Prevent instantiation
    ~GameRuleEngine() = default;
};

#endif