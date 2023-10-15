#include "GameInstance.h"
#include "GameConfigLoader.h"


GameInstance::GameInstance(std::shared_ptr<GameRules> gameRules, 
std::unique_ptr<GameState> gameState, int inviteCode)
    : m_gameRules(gameRules), m_gameState(std::move(gameState)), m_inviteCode(inviteCode)
{
    const ts::Node rules = gameRules->getRules();
    ts::Node body = rules.getNamedChild(0);

    std::shared_ptr<Rule> firstInstruction(RuleInterpreter::createRule(body, gameRules->getSource()));
    instructionStack.push(firstInstruction);
}

bool 
GameInstance::executeNextInstruction() {
    if (instructionStack.empty()) return false;

    std::shared_ptr<Rule> instruction = instructionStack.top();
    std::optional<ts::Node> nextInstructionNode = instruction->execute();

    if (nextInstructionNode.has_value()) {
        std::shared_ptr<Rule> nextInstruction(
                RuleInterpreter::createRule(nextInstructionNode, m_gameRules->getSource()));

        instructionStack.push(nextInstruction);
    } else {
        instructionStack.pop();
    }

    return true;
}

int 
GameInstance::getInviteCode() {
    return m_inviteCode;
}