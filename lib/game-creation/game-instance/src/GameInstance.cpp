#include "GameInstance.h"
#include "GameConfigLoader.h"

GameInstance::GameInstance(std::unique_ptr<RuleTree> gameRules, 
std::unique_ptr<GameState> gameState, int inviteCode)
    : m_gameRules(std::move(gameRules)), m_gameState(std::move(gameState)), m_inviteCode(inviteCode)
{
    // const std::shared_ptr<RuleNode> rulesRoot = gameRules->getRules();
    // instructionStack.push(rulesRoot);

    // UNIMPLEMENTED While Rule Execution is restructured
}

void 
GameInstance::startGame() {
    // UNIMPLEMENTED While Rule Execution is restructured

    // std::cout << "\nGame started." << std::endl;
    // while (!instructionStack.empty()) {
    //     executeNextInstruction();
    // }
    // std::cout << "Game ended." << std::endl;
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

int 
GameInstance::getInviteCode() {
    return m_inviteCode;
}