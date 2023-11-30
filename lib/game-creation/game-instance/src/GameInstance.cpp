#include "GameInstance.h"
#include "GameConfigLoader.h"

#include <iostream> // TODO: delete this

GameInstance::GameInstance(std::unique_ptr<RuleTree> gameRules, 
std::unique_ptr<GameState> gameState, std::unique_ptr<GameSetup> gameSetup, uint16_t roomCode)
    : m_gameRules(std::move(gameRules)), 
      m_gameState(std::move(gameState)),
      m_gameSetup(std::move(gameSetup)),
      m_roomCode(roomCode),
      m_context(*m_gameState, m_gameRules->getRoot()),
      m_ruleExecutor(m_context)
{
    m_inGameUserManager = std::make_unique<InGameUserManager>();
    // const std::shared_ptr<RuleNode> rulesRoot = gameRules->getRules();
    // instructionStack.push(rulesRoot);

    m_state = GameInstanceState::QUEUED;
    if (!m_gameSetup->hasSetup()) {
        m_setupIndex = SETUP_FINISHED;
    }
}

ConfigResult
GameInstance::inputConfig(const std::string& response) {
    std::vector<std::string_view> identifiers = m_gameSetup->getIdentifiers();
    std::vector<std::string_view> prompts = m_gameSetup->getPrompts();

    

    if (m_setupIndex == identifiers.size()) {
        m_setupIndex = SETUP_FINISHED;
        return ConfigResult{"Finished setup.\n", true, true};
    }

    return ConfigResult{"Invalid.\n", false, false};
}

void 
GameInstance::startGame() {
    m_state = GameInstanceState::RUNNING;
}

void 
GameInstance::executeNextInstruction() {
    while (!m_context.blocked && (m_context.instructionStack.size() > 0)) {
        m_context.instructionStack.top()->accept(m_ruleExecutor);
    }
    if (m_context.blocked) {
        m_state = GameInstanceState::WAITING;
    }
}

bool
GameInstance::gameIsFinished() {
    if (m_context.instructionStack.empty()) {
        return true;
    }
    return false;
}

uint16_t 
GameInstance::getRoomCode() {
    return m_roomCode;
}

void
GameInstance::addUsers(const std::vector<User>& users) {
    for (const User& user : users) {
        GameEnvironment::Environment dummyEnvironment;

        // TODO: Figure out where/how to insert an Identifier/Value key-value mapping
        // and then access it in InGameUserManager. The code below seems to leave the key-value pairing 
        // as nullptrs, but their declaration doesn't necessarily have to come from GameInstance itself.

        // std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> tester;
        // GameEnvironment::Value testValue;
        // testValue.value = 500;
        // std::unique_ptr<GameEnvironment::Value> testValuePtr;
        // testValuePtr -> value;
        // std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
        // GameEnvironment::Identifier testIdentifier = "testidentifier";
        // testMap.insert({testIdentifier, std::move(testValuePtr)});
        // std::cout << "Before adding, user ID is:" << user.userID.id << "\n";
        //m_inGameUserManager->addNewUser(user.userID, testMap);
        m_inGameUserManager->addNewUser(user.userID, std::move(dummyEnvironment));
    }
}

void
GameInstance::deleteUsers(const std::vector<User>& users) {
    for (const User& user : users) {
        m_inGameUserManager->deleteUser(user.userID);
    }
}

GameInstanceState
GameInstance::getGameInstanceState() {
    return m_state;
}

bool 
GameInstance::gameIsJoinable() {
    return m_setupIndex == SETUP_FINISHED;
}

bool
GameInstance::gameHasSetup() {
    return m_gameSetup->hasSetup();
}