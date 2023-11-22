#include "GameInstance.h"
#include "GameConfigLoader.h"

GameInstance::GameInstance(std::unique_ptr<RuleTree> gameRules, 
std::unique_ptr<GameState> gameState, uint16_t roomCode)
    : m_gameRules(std::move(gameRules)), m_gameState(std::move(gameState)), m_roomCode(roomCode)
{
    // const std::shared_ptr<RuleNode> rulesRoot = gameRules->getRules();
    // instructionStack.push(rulesRoot);

    // UNIMPLEMENTED While Rule Execution is restructured
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

uint16_t 
GameInstance::getRoomCode() {
    return m_roomCode;
}

void
GameInstance::addUsers(const std::vector<User>& users) { 
    // TODO: Commented out because this results in a seg-fault - need to fix

    // for (const User& user : users) {
    //     GameEnvironment::Environment dummyEnvironment;
    //     m_inGameUserManager->addNewUser(user.userID, std::move(dummyEnvironment));
    // }
}

void
GameInstance::deleteUsers(const std::vector<User>& users) {
    for (const User& user : users) {
        m_inGameUserManager->deleteUser(user.userID);
    }
}