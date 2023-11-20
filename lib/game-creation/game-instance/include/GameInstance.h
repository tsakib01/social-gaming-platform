#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "Translator.h"
#include "GameState.h"
#include "UserManager.h"
#include <stack>
#include <iostream>

enum class GameInstanceState : char {
    QUEUED,
    RUNNING,
    WAITING
};

class GameInstance {
public:
    GameInstance(std::unique_ptr<RuleTree> rules, std::unique_ptr<GameState> state, uint16_t m_roomCode);
    ~GameInstance() {};

    void startGame();
    void executeNextInstruction();
    bool gameIsFinished();
    void insertUsers(std::vector<User>& incomingUsers);
    void insertMessages();
    uint16_t getRoomCode();
    GameInstanceState getGameInstanceState();
    
    // Flips between WAITING and RUNNING states (becomes whichever it isn't currently)
    void flipWaitingRunningState();


private:
    std::unique_ptr<RuleTree> m_gameRules;
    std::unique_ptr<GameState> m_gameState;
    // std::stack<std::shared_ptr<RuleNode>> instructionStack;
    
    uint16_t m_roomCode;
    GameInstanceState m_state;
};

#endif