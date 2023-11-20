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

    // TODO: find some way to detect if there was an input rule happening
    // if true, m_state = GameInstanceState::WAITING;
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
    // add to InGameUserManager
}

void
GameInstance::insertMessages() { 
    m_state = GameInstanceState::RUNNING;
    
    // Pass messages to rule
}

uint16_t 
GameInstance::getRoomCode() {
    return m_roomCode;
}

GameInstanceState
GameInstance::getGameInstanceState() {
    return m_state;
}