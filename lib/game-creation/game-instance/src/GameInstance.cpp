#include "GameInstance.h"
#include "GameConfigLoader.h"

GameInstance::GameInstance(std::unique_ptr<RuleTree> gameRules, 
std::unique_ptr<GameState> gameState, uint16_t roomCode)
    : m_gameRules(std::move(gameRules)), m_gameState(std::move(gameState)), m_roomCode(roomCode)
{
    // const std::shared_ptr<RuleNode> rulesRoot = gameRules->getRules();
    // instructionStack.push(rulesRoot);

    // UNIMPLEMENTED While Rule Execution is restructure

    m_state = GameInstanceState::QUEUED;
}

void 
GameInstance::startGame() {
    m_state = GameInstanceState::RUNNING;
}

void 
GameInstance::executeNextInstruction() {
    // UNIMPLEMENTED While Rule Execution is restructured

    // if (instructionStack.empty()) return;

    // std::shared_ptr<RuleNode> instruction = instructionStack.top();
    // std::optional<std::shared_ptr<RuleNode>> nextInstructionNode = instruction->executeRule();

    // if (nextInstructionNode.has_value()) {
    //     instructionStack.push(nextInstructionNode.value());
    // } else {
    //     instructionStack.pop();
    // }
}

bool
GameInstance::gameIsFinished() {
    // UNIMPLEMENTED While Rule Execution is restructured

    // if (instructionStack.empty()) {
    //     m_gameRules->getRules()->deleteReferences();
    //     return true;
    // }
    return false;
}

void
GameInstance::insertUsers(std::vector<User>& incomingUsers) {
    // TODO: Pass into InGameUserManager
}

void
GameInstance::insertMessages() { 
    flipState();
    
    // TODO: Pass messages to rules
}

uint16_t 
GameInstance::getRoomCode() {
    return m_roomCode;
}

GameInstanceState
GameInstance::getGameInstanceState() {
    return m_state;
}

void
GameInstance::flipState() {
    if (m_state == GameInstanceState::QUEUED) {
        throw std::runtime_error("GameInstanceState should not be QUEUED!");
    }

    m_state = (m_state == GameInstanceState::RUNNING) ? GameInstanceState::WAITING : GameInstanceState::RUNNING;
}