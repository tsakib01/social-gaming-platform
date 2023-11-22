#include "GameInstance.h"
#include "GameConfigLoader.h"

GameInstance::GameInstance(std::unique_ptr<RuleTree> gameRules, 
std::unique_ptr<GameState> gameState, uint16_t roomCode)
    : m_gameRules(std::move(gameRules)), m_gameState(std::move(gameState)), m_roomCode(roomCode)
{
    // const std::shared_ptr<RuleNode> rulesRoot = gameRules->getRules();
    // instructionStack.push(rulesRoot);

    // UNIMPLEMENTED While Rule Execution is restructured

    gameInstanceState = GameInstanceState::QUEUED;
}

void 
GameInstance::startGame() {
    // UNIMPLEMENTED While Rule Execution is restructured

    // std::cout << "\nGame started." << std::endl;
    // while (!instructionStack.empty()) {
    //     executeNextInstruction();
    // }
    // std::cout << "Game ended." << std::endl;

    gameInstanceState = gameInstanceState::RUNNING;
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
GameInstance::flipRunWaitState() {
    if (m_gameInstanceState == GameInstanceState::QUEUED) {
        throw std::runtime_error("Shouldn't use this in QUEUED state.");
    }

    m_state = (m_state == GameInstanceState::RUNNING) ? GameInstanceState::WAITING : GameInstanceState::RUNNING;
}

uint16_t 
GameInstance::getRoomCode() {
    return m_roomCode;
}

GameInstanceState
GameInstance::getGameInstanceState() {
    return m_gameInstanceState;
}