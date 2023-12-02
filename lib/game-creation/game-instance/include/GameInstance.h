#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "Translator.h"
#include "GameState.h"
#include "InGameUserManager.h"
#include "RuleExecute.h"
#include "GameSetup.h"
#include <stack>
#include <iostream>

enum class GameInstanceState {
    QUEUED,
    RUNNING,
    WAITING
};

struct ValidResponse {
    bool status;
};

struct Finished {
    bool status;
};

struct ConfigResult {
    std::string prompt;
    ValidResponse validResponse;
    Finished finished;
};

const int SETUP_FINISHED = -1;

class GameInstance {
public:
    GameInstance(std::unique_ptr<RuleTree> rules, std::unique_ptr<GameState> state, 
        std::unique_ptr<GameSetup> gameSetup, uint16_t m_roomCode);
    ~GameInstance() {};

    ConfigResult inputConfig(const std::string& response);

    void startGame();
    bool gameIsFinished();
    void executeNextInstruction(); 

    uint16_t getRoomCode();
    GameInstanceState getGameInstanceState();

    void addUsers(const std::vector<User>& users);
    void deleteUsers(const std::vector<User>& users);
    void updateUserStates(const std::vector<User>& users, GameEnvironment::Environment environmentToUse);
    std::map<uintptr_t, GameEnvironment::Environment> getUserStates();
    bool gameIsJoinable();
    bool gameHasSetup();

private:
    std::unique_ptr<RuleTree> m_gameRules;
    std::unique_ptr<GameState> m_gameState;
    std::unique_ptr<GameSetup> m_gameSetup;
    std::unique_ptr<InGameUserManager> m_inGameUserManager;
    
    GameInstanceState m_state;
    ExecuteContext m_context;
    RuleExecuteVisitor m_ruleExecutor;

    uint16_t m_roomCode;
    int m_setupIndex = 0;
    bool sentFirstPrompt = false;
    std::vector<std::pair<std::string, std::string>> m_setupResponses;
};

#endif