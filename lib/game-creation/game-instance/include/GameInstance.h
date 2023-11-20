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

    // Sets State to QUEUED
    void startGame();

    // Executes the top-most instruction
    void executeNextInstruction();

    // Deletes any references (may delete - is this necessary?)
    bool gameIsFinished();

    // Insert a list of users into the InGameUserManager
    void insertUsers(std::vector<User>& incomingUsers);

    // Insert a list of messages for an Input Rule
    void insertMessages();

    // Returns the unique 4-digit room code
    uint16_t getRoomCode();

    // Returns the current state of the room
    GameInstanceState getGameInstanceState();
    
    // Flips between WAITING and RUNNING states (becomes whichever it isn't currently)
    void flipState();


private:
    std::unique_ptr<RuleTree> m_gameRules;
    std::unique_ptr<GameState> m_gameState;
    // std::stack<std::shared_ptr<RuleNode>> instructionStack;
    
    uint16_t m_roomCode;
    GameInstanceState m_state;
};

#endif