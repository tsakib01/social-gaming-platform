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
    for (const User& user : users) {
        GameEnvironment::Environment dummyEnvironment;
        std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> tester;
        GameEnvironment::Value testValue;
        testValue.value = 500;
        std::unique_ptr<GameEnvironment::Value> testValuePtr;
        testValuePtr -> value;
        std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
        GameEnvironment::Identifier testIdentifier = "testidentifier";
        testMap.insert({testIdentifier, std::move(testValuePtr)});
        std::cout << "Before adding, user ID is:" << user.userID.id << "\n";
        m_inGameUserManager.addNewUser(user.userID, std::move(testMap));
    }

    std::map<uintptr_t, GameEnvironment::Environment> daMap = m_inGameUserManager.getAllUserStates();
    for (auto const &pair: daMap){
        std::cout << "Oooo look at the person: " << pair.first << "\n";
    }
    //deleteUsers(users);
}

void
GameInstance::deleteUsers(const std::vector<User>& users) {
    
    std::map<uintptr_t, GameEnvironment::Environment> daMap = std::move(m_inGameUserManager.getAllUserStates());
    // The returned map is empty for some reason...
    for (auto const &pair: daMap){
        std::cout << "Here is a person's data which should be getting deleted: " << pair.first << "\n";
    }
    for (const User& user : users) {
        m_inGameUserManager.deleteUser(user.userID);
    }
    for (auto const &pair: daMap){
        std::cout << "This person should have been deleted by now: " << pair.first << "\n";
    }
}