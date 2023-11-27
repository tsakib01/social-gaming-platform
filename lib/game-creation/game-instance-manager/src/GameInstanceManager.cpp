#include "GameInstanceManager.h"

GameInstanceManager::GameInstanceManager() {}

uint16_t 
GameInstanceManager::generateRoomCode() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<uint16_t> distribution(1000, 9999);

    while (true) {
        uint16_t inviteCode = distribution(generator);

        auto codeExists = std::find_if(m_gameList.begin(), m_gameList.end(), 
            [inviteCode](const std::unique_ptr<GameInstance>& game) {
                return game->getRoomCode() == inviteCode;
            });

        if (codeExists == m_gameList.end()) {
            return inviteCode;
        }
    }
}

uint16_t 
GameInstanceManager::createGameInstance(std::string_view gameFilePath) {
    GameConfigLoader gameConfigLoader{gameFilePath};
    auto rules = gameConfigLoader.createGameRules();
    auto state = gameConfigLoader.createGameState();
    uint16_t inviteCode = generateRoomCode();

    m_gameList.push_back(std::make_unique<GameInstance>(std::move(rules), std::move(state), inviteCode));

    return inviteCode;
}

void
GameInstanceManager::startGame(uint16_t roomCode, const std::vector<User>& users) {
    // Move from gameList to activeGameList
    addUsersToGame(roomCode, users);

    auto it = std::find_if(m_gameList.begin(), m_gameList.end(), [roomCode](const std::unique_ptr<GameInstance>& game) {
        return game->getRoomCode() == roomCode;
    });

    if (it != m_gameList.end()) {
        (*it)->startGame();
    }
}

void 
GameInstanceManager::runCycle() {
    while (!m_gameList.empty()) {
        for (std::unique_ptr<GameInstance>& game : m_gameList) {
            game->executeNextInstruction();
        }
        m_gameList.erase(std::remove_if(m_gameList.begin(), m_gameList.end(),
            [](const std::unique_ptr<GameInstance>& game) {
                return game->gameIsFinished();
            }), m_gameList.end());
    }
    // After a game finishes exeuction (until an input), call gameInstance.flipRunWaitState()
}

std::vector<uint16_t> 
GameInstanceManager::getRoomCodes() {
    std::vector<uint16_t> roomCodes;
    std::transform(m_gameList.begin(), m_gameList.end(), std::back_inserter(roomCodes),
        [](const std::unique_ptr<GameInstance>& gameInstance) {
            return gameInstance->getRoomCode();
        });
    return roomCodes;
}

void
GameInstanceManager::addUsersToGame(uint16_t roomCode, const std::vector<User>& users) {
    auto game = getGameIterator(roomCode);
    (*game)->addUsers(users);
}

void 
GameInstanceManager::deleteUsersFromGame(uint16_t roomCode, const std::vector<User>& users){
    auto game = getGameIterator(roomCode);
    (*game)->deleteUsers(users);
}

std::vector<std::unique_ptr<GameInstance>>::iterator 
GameInstanceManager::getGameIterator(uint16_t roomCode) {
    auto game = std::find_if(m_gameList.begin(), m_gameList.end(), [roomCode](const std::unique_ptr<GameInstance>& gameInstance) {
        return gameInstance->getRoomCode() == roomCode;
    });

    if (game == m_gameList.end()) {
        throw std::runtime_error("Game was not found.");
    }

    return game;
} 