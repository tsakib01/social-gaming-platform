#include <iostream>
#include <cpp-tree-sitter.h>
#include "GameConfigLoader.h"
#include "GameInstance.h"
#include "GameInstanceManager.h"


int main(int argc, char** argv) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [game file input]\n";
        return EXIT_FAILURE;
    }

    std::string_view gameFilePath = argv[1];
    std::cout << "Game file path: " << gameFilePath << "\n";

    // TODO: Creating GameInstances with configloader should probably be handled by GIM
    // GameConfigLoader gameConfigLoader{gameFilePath};
    // auto setup = gameConfigLoader.createGameSetup();
    // setup->processGameSetups();
    GameInstanceManager gameInstanceManager = GameInstanceManager();
    gameInstanceManager.createGameInstance(gameFilePath);
    gameInstanceManager.runCycle();

    return EXIT_SUCCESS;  

}
