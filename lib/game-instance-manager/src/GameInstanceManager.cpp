#include "GameInstanceManager.h"

GameInstanceManager::GameInstanceManager() {
    std::cout << "GameInstanceManager created." << std::endl;
}

int 
GameInstanceManager::generateInviteCode() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(1000, 9999);

    while (true) {
        int inviteCode = distribution(generator);

        auto codeExists = std::find_if(m_gameList.begin(), m_gameList.end(), 
            [inviteCode](const std::unique_ptr<GameInstance>& game) {
                return game->getInviteCode() == inviteCode;
            });

        if (codeExists == m_gameList.end()) {
            return inviteCode;
        }
    }
}

void 
GameInstanceManager::sendInviteCode() {
    // Blocked until there's a MessageHandler

    // The MessageHandler should be able to ask for the invite code
    // GameInstanceManager should return the invite code for that specified / created game
}

void 
GameInstanceManager::createGameInstance(std::string_view gameFilePath) {
    GameConfigLoader gameConfigLoader{gameFilePath};
    auto rules = gameConfigLoader.createGameRules();
    auto state = gameConfigLoader.createGameState();
    int inviteCode = generateInviteCode();

    m_gameList.push_back(std::make_unique<GameInstance>(rules, std::move(state), inviteCode));
}

void 
GameInstanceManager::startGame() {
    // Blocked until there's a MessageHandler
}

void 
GameInstanceManager::finishGame() {
    // Should remove the game from both m_gameList and m_activeGameList
}

void 
GameInstanceManager::runCycle() {
    while (true) {
        for (std::unique_ptr<GameInstance>& game : m_gameList) {
            if (!(game->executeNextInstruction())) {
                m_gameList.erase(std::remove(m_gameList.begin(), m_gameList.end(), game));
            }
        }

        // TODO There shouldn't be a break here, replace once we have working networking
        if (m_gameList.empty()) break; 
    }
}