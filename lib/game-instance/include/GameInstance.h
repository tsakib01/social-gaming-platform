#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "RuleInterpreter.h"
#include "GameRules.h"
#include "GameState.h"
#include <stack>
#include <iostream>

class GameInstance {
public:
    GameInstance(std::shared_ptr<GameRules> rules, std::unique_ptr<GameState> state);
    ~GameInstance() {};
    void startGame();
    void executeNextInstruction();

private:
    std::shared_ptr<GameRules> m_gameRules;
    std::unique_ptr<GameState> m_gameState;
    std::stack<std::shared_ptr<Rule>> instructionStack;
};

#endif