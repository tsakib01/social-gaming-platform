#include "GameInstance.h"
#include "GameConfigLoader.h"
#include "RuleExecute.h"

GameInstance::GameInstance(std::unique_ptr<RuleTree> gameRules, 
std::unique_ptr<GameState> gameState, uint16_t roomCode)
    : m_gameRules(std::move(gameRules)), m_gameState(std::move(gameState)), m_roomCode(roomCode)
{
    // const std::shared_ptr<RuleNode> rulesRoot = gameRules->getRules();
    // instructionStack.push(rulesRoot);

    // UNIMPLEMENTED While Rule Execution is restructured
}

void 
GameInstance::startGame() {
    ExecuteContext context{*m_gameState, m_gameRules->getRoot()};
    RuleExecuteVisitor ruleExecutor(context);

    std::cout << "\nGame started." << std::endl;
    while (!context.blocked && (context.instructionStack.size() > 0)) {
        context.instructionStack.top()->accept(ruleExecutor);
    }

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

uint16_t 
GameInstance::getRoomCode() {
    return m_roomCode;
}