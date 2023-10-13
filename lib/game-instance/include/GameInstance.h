#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "RuleInterpreter.h"
#include "GameRules.h"
#include "GameState.h"
#include <stack>
#include <iostream>

class GameInstance {
public:
    GameInstance(std::unique_ptr<GameRules> rules, std::unique_ptr<GameState> state);
    ~GameInstance() {};
    void startGame();
    void executeNextInstruction();

private:
    std::unique_ptr<GameRules> gameRules;
    std::unique_ptr<GameState> gameState;
    std::stack<std::shared_ptr<Rule>> instructionStack;
    std::string_view source;
};

#endif