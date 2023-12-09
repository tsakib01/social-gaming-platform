#include "GameInstance.h"
#include "GameConfigLoader.h"
#include "GameEnvironment.h"
#include "OutgoingMessages.h"

GameInstance::GameInstance(std::unique_ptr<RuleTree> gameRules, std::unique_ptr<GameState> gameState, 
std::unique_ptr<GameSetup> gameSetup, GameCommunicator& gameCommunicator, uint16_t roomCode)
    : m_gameRules(std::move(gameRules)), 
      m_gameState(std::move(gameState)),
      m_gameSetup(std::move(gameSetup)),
      m_gameCommunicator(gameCommunicator),
      m_roomCode(roomCode),
      m_context(*m_gameState, OutgoingMessages({}), m_gameRules->getRoot()),
      m_ruleExecutor(m_context)
{
    m_inGameUserManager = std::make_unique<InGameUserManager>();
    m_state = GameInstanceState::QUEUED;
    
    if (!gameHasSetup()) {
        m_setupIndex = SETUP_FINISHED;
    }
}

ConfigResult
GameInstance::inputConfig(const std::string& response) {
    std::vector<std::string_view> identifiers = m_gameSetup->getIdentifiers();
    std::vector<std::string_view> prompts = m_gameSetup->getPrompts();
    std::vector<std::string_view> restInfos = m_gameSetup->getRestInfos();

    if (!sentFirstPrompt) {
        sentFirstPrompt = true;
        return ConfigResult{
            "Configuration Setup...\nEnter " + std::string(prompts[m_setupIndex]), ValidResponse{false}, Finished{false}};
    }

    if (m_gameSetup->isResponseValid(identifiers[m_setupIndex], response)) {
        m_setupResponses.push_back({(identifiers[m_setupIndex]), response});
        m_setupIndex++;
        if (m_setupIndex == identifiers.size()) {
            m_setupIndex = SETUP_FINISHED;
            // TODO: Can insert setupResponses into gameState here.
            addSetupIntoState();

//            m_gameState->print();
            return ConfigResult{
                "Finished setup.\n", ValidResponse{true}, Finished{true}};
        }
        return ConfigResult{
            "Enter " + std::string(prompts[m_setupIndex]), ValidResponse{true}, Finished{false}};
    } 
    
    return ConfigResult{
        "Invalid.\n", ValidResponse{false}, Finished{false}};
}

void 
GameInstance::startGame() {
    m_state = GameInstanceState::RUNNING;
}

void 
GameInstance::execute() {
    while (!m_context.blocked && (m_context.instructionStack.size() > 0)) {
        m_context.instructionStack.top()->accept(m_ruleExecutor);
    }
    if (m_context.blocked) {
        m_state = GameInstanceState::WAITING;
        
        // TODO: Replace this once executeContext has outgoingMessages
        auto outgoing = m_context.outgoingMessages;
        m_gameCommunicator.setGameMessage(outgoing.getMessages());
        outgoing.clear();
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

// TODO: Figure out where/how to insert an Identifier/Value key-value mapping
// and then access it in InGameUserManager. The code below seems to leave the key-value pairing 
// as nullptrs, but their declaration doesn't necessarily have to come from GameInstance itself.
void
GameInstance::addUsers(const std::vector<User>& users) {
    for (const User& user : users) {
        GameEnvironment::Environment dummyEnvironment;
        m_inGameUserManager->addNewUser(user.userID, std::move(dummyEnvironment));
        m_gameState->addPlayerState(user);
        
        auto users = m_inGameUserManager->getAllUserIds();
        m_context.outgoingMessages = OutgoingMessages{users};
    }
}

void
GameInstance::deleteUsers(const std::vector<User>& users) {
    for (const User& user : users) {
        m_inGameUserManager->deleteUser(user.userID);
    }
}


// TODO: Figure out where/how to insert an Identifier/Value key-value mapping
// and then access it in InGameUserManager. The code below seems to leave the key-value pairing 
// as nullptrs, but their declaration doesn't necessarily have to come from GameInstance itself.
// void
// GameInstance::updateUserStates(const std::vector<User>& users) {
//         for (const User& user : users) {
//         // This Enviroment has to be taken from somewhere...
//         m_inGameUserManager->setStatesOfUser(user.userID, std::move(environmentToUse));
//     }
// }

std::map<uintptr_t, GameEnvironment::Environment> GameInstance::getUserStates(){
    return m_inGameUserManager->getAllUserStates();
}

GameInstanceState
GameInstance::getGameInstanceState() {
    return m_state;
}

bool 
GameInstance::gameIsJoinable() {
    return m_setupIndex == SETUP_FINISHED;
}

bool
GameInstance::gameHasSetup() {
    return m_gameSetup->hasSetup();
}

GameEnvironment::Value convertSetupResponseToValue( KIND kind, std::string_view response ){

    if(kind == KIND::INTEGER){
        return GameEnvironment::Value(std::stoi(std::string(response)));
    }
    else if(kind == KIND::BOOLEAN){
        bool value ;
        if (response=="y"){
            value = true;
        }
        else{
            value = false;
        }
        return GameEnvironment::Value(value);
    }
    else{
        return GameEnvironment::Value(response);
    }
}

void GameInstance::addSetupIntoState(){
    int size = m_setupResponses.size();
    for(int i=0; i<size; i++){
        std::string_view identifier = m_setupResponses[i].first;
        auto kind = m_gameSetup->getKind(identifier);
        auto value = convertSetupResponseToValue(kind, m_setupResponses[i].second );
        m_gameState->addSetupToGameState(identifier,value);
    }
}