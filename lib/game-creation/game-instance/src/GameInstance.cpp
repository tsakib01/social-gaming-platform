#include "GameInstance.h"
#include "GameConfigLoader.h"

GameInstance::GameInstance(std::unique_ptr<RuleTree> gameRules, 
std::unique_ptr<GameState> gameState, uint16_t roomCode)
    : m_gameRules(std::move(gameRules)), 
      m_gameState(std::move(gameState)),
      m_roomCode(roomCode),
      m_context(*m_gameState, m_gameRules->getRoot()),
      m_ruleExecutor(m_context)
{
    m_state = GameInstanceState::QUEUED;
}

void 
GameInstance::startGame() {
    std::cout << "\nGame started." << std::endl;
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

GameInstanceState
GameInstance::getGameInstanceState() {
    return m_state;
}