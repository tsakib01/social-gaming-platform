#include "GameInstance.h"
#include "GameConfigLoader.h"
#include "RuleNode.h"

GameInstance::GameInstance(std::shared_ptr<GameRules> gameRules, 
std::unique_ptr<GameState> gameState, int inviteCode)
    : m_gameRules(gameRules), m_gameState(std::move(gameState)), m_inviteCode(inviteCode)
{
    const std::shared_ptr<RuleNode> rulesRoot = gameRules->getRules();
    startGame(rulesRoot);
}

void 
GameInstance::startGame(std::shared_ptr<RuleNode> rulesRoot) {

    instructionStack.push(rulesRoot);
    // This function shouldn't exist - it's just a test to see if it will work
    // Should probably be in GameInstanceManager

    std::cout << "\nGame started." << std::endl;
    while (!instructionStack.empty()) {
        executeNextInstruction();
    }
    std::cout << "Game ended." << std::endl;
    rulesRoot->deleteReferences();
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
    return instructionStack.empty();
}

int 
GameInstance::getInviteCode() {
    return m_inviteCode;
}