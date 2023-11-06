#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "Translator.h"
#include "GameState.h"
#include <stack>
#include <iostream>

class GameInstance {
public:
    GameInstance(std::unique_ptr<RuleTree> rules, std::unique_ptr<GameState> state, uint16_t m_roomCode);
    ~GameInstance() {};
    void startGame();
    void executeNextInstruction();
    bool gameIsFinished();
    uint16_t getRoomCode();

private:
    std::unique_ptr<RuleTree> m_gameRules;
    std::unique_ptr<GameState> m_gameState;
    // std::stack<std::shared_ptr<RuleNode>> instructionStack;
    uint16_t m_roomCode;
};

#endif