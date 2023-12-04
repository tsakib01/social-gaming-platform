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
    std::unique_ptr<GameConfigLoader> gameConfigLoader = std::make_unique<GameConfigLoader>(gameFilePath);
    auto rules = gameConfigLoader->createGameRules();
    auto state = gameConfigLoader->createGameState();
    auto setup = gameConfigLoader->createGameSetup();
    uint16_t inviteCode = generateRoomCode();

    m_configs.push_back(std::move(gameConfigLoader));
    m_gameList.push_back(std::make_unique<GameInstance>(std::move(rules), std::move(state), std::move(setup), inviteCode));

    return inviteCode;
}

ConfigResult
GameInstanceManager::inputConfig(uint16_t roomCode, const std::string& response) {
    auto& game = getGameInstance(roomCode);
    return game->inputConfig(response);
}

void
GameInstanceManager::startGame(uint16_t roomCode, const std::vector<User>& users) {
    auto& game = getGameInstance(roomCode);
    game->addUsers(users);
    game->startGame();
}

void
GameInstanceManager::deleteGame(uint16_t roomCode) {
    auto gameIterator = std::remove_if(m_gameList.begin(), m_gameList.end(),
        [roomCode](const std::unique_ptr<GameInstance>& gameInstance) {
            return gameInstance->getRoomCode() == roomCode;
        });

    m_gameList.erase(gameIterator, m_gameList.end());
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
    auto& game = getGameInstance(roomCode);
    game->addUsers(users);
}

void 
GameInstanceManager::deleteUsersFromGame(uint16_t roomCode, const std::vector<User>& users){
    auto& game = getGameInstance(roomCode);
    game->deleteUsers(users);
}

bool 
GameInstanceManager::gameIsJoinable(uint16_t roomCode) {
    auto& game = getGameInstance(roomCode);
    return game->gameIsJoinable();
}

bool
GameInstanceManager::gameHasSetup(uint16_t roomCode) {
    auto& game = getGameInstance(roomCode);
    return game->gameHasSetup();
}

std::unique_ptr<GameInstance>& 
GameInstanceManager::getGameInstance(uint16_t roomCode) {
    auto game = std::find_if(m_gameList.begin(), m_gameList.end(), [roomCode](const std::unique_ptr<GameInstance>& gameInstance) {
        return gameInstance->getRoomCode() == roomCode;
    });

    if (game == m_gameList.end()) {
        throw std::runtime_error("Game was not found.");
    }

    return *game;
} 