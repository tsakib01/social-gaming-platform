#include <iostream>
#include "GameConfigLoader.h"
#include "GameInstance.h"

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [game file input]\n";
        return EXIT_FAILURE;
    }

    std::string_view gameFilePath = argv[1];
    std::cout << "Game file path: " << gameFilePath << "\n";

    // TODO: Creating GameInstances with configloader should probably be handled by GIM
    GameConfigLoader gameConfigLoader{};
    auto rules = gameConfigLoader.createGameRules(gameFilePath);
    auto state = gameConfigLoader.createGameState();

    GameInstance game = GameInstance(rules, state);

    return EXIT_SUCCESS;    
}