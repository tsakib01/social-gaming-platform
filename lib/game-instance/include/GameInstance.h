#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "RuleInterpreter.h"
#include "GameRules.h"
#include "GameState.h"
#include "UserManager.h"
#include <stack>
#include <iostream>

class GameInstance {
public:
    GameInstance(std::shared_ptr<GameRules> rules, std::unique_ptr<GameState> state, int roomCode);
    ~GameInstance() {};
    void startGame();
    void executeNextInstruction();
    bool gameIsFinished();
    uint16_t getRoomCode();

    void insertUsers(std::vector<User> users);

private:
    std::shared_ptr<GameRules> m_gameRules;
    std::unique_ptr<GameState> m_gameState;
    std::stack<std::shared_ptr<RuleNode>> instructionStack;
    uint16_t m_roomCode;

    std::vector<User> m_users;
};

#endif