#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "RuleInterpreter.h"
#include "GameRules.h"
#include "GameState.h"
#include <stack>
#include <iostream>

class GameInstance {
public:
    GameInstance(std::shared_ptr<GameRules> rules, std::unique_ptr<GameState> state, int inviteCode);
    ~GameInstance() {};
    bool executeNextInstruction();
    int getInviteCode();

private:
    std::shared_ptr<GameRules> m_gameRules;
    std::unique_ptr<GameState> m_gameState;
    std::stack<std::shared_ptr<Rule>> instructionStack;
    int m_inviteCode;
};

#endif