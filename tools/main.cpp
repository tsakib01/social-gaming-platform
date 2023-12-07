#include <iostream>
#include <cpp-tree-sitter.h>
#include "GameConfigLoader.h"
#include "GameInstance.h"
#include "GameInstanceManager.h"
#include "UserManager.h"
#include "Server.h"


int main(int argc, char** argv) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [game file input]\n";
        return EXIT_FAILURE;
    }

    std::string_view gameFilePath = argv[1];
    std::cout << "Game file path: " << gameFilePath << "\n";

    // TODO: Creating GameInstances with configloader should probably be handled by GIM
    GameConfigLoader gameConfigLoader{gameFilePath};
    auto setup = gameConfigLoader.createGameSetup();
    setup->processGameSetups();
    auto identifers=setup->getIdentifiers();

    GameInstanceManager gameInstanceManager = GameInstanceManager();
    GameCommunicator gameCommunicator;
    auto invCode = gameInstanceManager.createGameInstance(gameFilePath, gameCommunicator);
    User user{networking::Connection{1}};
    user.username = "testUser";
    std::vector<User> users;
    users.emplace_back(user);
    gameInstanceManager.startGame(invCode, users);
    gameInstanceManager.runCycle();
    return EXIT_SUCCESS;  

}
