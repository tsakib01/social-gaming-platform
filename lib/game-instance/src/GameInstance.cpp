#include "GameInstance.h"
#include "GameConfigLoader.h"
#include "RuleNode.h"

GameInstance::GameInstance(std::shared_ptr<GameRules> gameRules, 
std::unique_ptr<GameState> gameState, int roomCode)
    : m_gameRules(gameRules), m_gameState(std::move(gameState)), m_roomCode(roomCode)
{
    const std::shared_ptr<RuleNode> rulesRoot = gameRules->getRules();
    instructionStack.push(rulesRoot);
}

void 
GameInstance::startGame() {

    // This function shouldn't exist - it's just a test to see if it will work
    // Should probably be in GameInstanceManager

    std::cout << "\nGame started." << std::endl;
    while (!instructionStack.empty()) {
        executeNextInstruction();
    }
    std::cout << "Game ended." << std::endl;
}

void 
GameInstance::executeNextInstruction() {
    if (instructionStack.empty()) return;

    std::shared_ptr<RuleNode> instruction = instructionStack.top();
    std::optional<std::shared_ptr<RuleNode>> nextInstructionNode = instruction->executeRule();

    if (nextInstructionNode.has_value()) {
        instructionStack.push(nextInstructionNode.value());
    } else {
        instructionStack.pop();
    }
}

bool
GameInstance::gameIsFinished() {
    if (instructionStack.empty()) {
        m_gameRules->getRules()->deleteReferences();
        return true;
    }
    return false;
}

uint16_t 
GameInstance::getRoomCode() {
    return m_roomCode;
}

void 
GameInstance::insertUsers(std::vector<std::unique_ptr<User>> users) {
    m_users = std::move(users);
}