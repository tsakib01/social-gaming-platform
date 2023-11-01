#include "GameInstanceManager.h"

GameInstanceManager::GameInstanceManager() {}

uint16_t 
GameInstanceManager::generateRoomCode() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<uint16_t> distribution(1000, 9999);

    while (true) {
        uint16_t roomCode = distribution(generator);

        auto codeExists = std::find_if(m_gameList.begin(), m_gameList.end(), 
            [roomCode](const std::unique_ptr<GameInstance>& game) {
                return game->getRoomCode() == roomCode;
            });

        if (codeExists == m_gameList.end()) {
            return roomCode;
        }
    }
}

void 
GameInstanceManager::createGameInstance(std::string_view gameFilePath) {
    GameConfigLoader gameConfigLoader{gameFilePath};
    auto rules = gameConfigLoader.createGameRules();
    auto state = gameConfigLoader.createGameState();
    uint16_t roomCode = generateRoomCode();

    m_gameList.push_back(std::make_unique<GameInstance>(std::move(rules), std::move(state), roomCode));
}

void 
GameInstanceManager::startGame() {
    // Blocked until there's a MessageHandler

    // The MessageHandler should get a signal from the owner to start and then call this function
    // Should add the game being started from m_gameList to m_activeGameList
}

void 
GameInstanceManager::finishGame() {
    // Should remove the game from both m_gameList and m_activeGameList
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

void 
GameInstanceManager::insertUsersIntoGame(std::vector<std::unique_ptr<User>> incomingUsers, uint16_t roomCode) {
    auto gameInstanceItr = std::find_if(m_gameList.begin(), m_gameList.end(), 
        [roomCode](const std::unique_ptr<GameInstance>& game) {
            return game->getRoomCode() == roomCode;
        });

    if (gameInstanceItr != m_gameList.end()) {
        (*gameInstanceItr)->insertUsers(std::move(incomingUsers));
    }
}