#include "GameInstance.h"
#include "GameConfigLoader.h"

GameInstance::GameInstance(std::unique_ptr<RuleTree> gameRules, 
std::unique_ptr<GameState> gameState, uint16_t roomCode)
    : m_gameRules(std::move(gameRules)), m_gameState(std::move(gameState)), m_roomCode(roomCode)
{
    m_inGameUserManager = std::make_unique<InGameUserManager>();
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
    for (const User& user : users) {
        GameEnvironment::Environment dummyEnvironment;

        // TODO: Figure out where/how to insert an Identifier/Value key-value mapping
        // and then access it in InGameUserManager. The code below seems to leave the key-value pairing 
        // as nullptrs, but their declaration doesn't necessarily have to come from GameInstance itself.

        // std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> tester;
        // GameEnvironment::Value testValue;
        // testValue.value = 500;
        // std::unique_ptr<GameEnvironment::Value> testValuePtr;
        // testValuePtr -> value;
        // std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
        // GameEnvironment::Identifier testIdentifier = "testidentifier";
        // testMap.insert({testIdentifier, std::move(testValuePtr)});
        // std::cout << "Before adding, user ID is:" << user.userID.id << "\n";
        //m_inGameUserManager->addNewUser(user.userID, testMap);
        m_inGameUserManager->addNewUser(user.userID, std::move(dummyEnvironment));
    }
}

void
GameInstance::deleteUsers(const std::vector<User>& users) {
    for (const User& user : users) {
        m_inGameUserManager->deleteUser(user.userID);
    }
}