#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "Translator.h"
#include "GameState.h"
#include "RuleExecute.h"
#include <stack>
#include <iostream>

enum class GameInstanceState {
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
    uint16_t getRoomCode();
    GameInstanceState getGameInstanceState();

private:
    std::unique_ptr<RuleTree> m_gameRules;
    std::unique_ptr<GameState> m_gameState;
    uint16_t m_roomCode;
    GameInstanceState m_state;
    ExecuteContext m_context;
    RuleExecuteVisitor m_ruleExecutor;
};

#endif