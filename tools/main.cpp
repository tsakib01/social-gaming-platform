#include <iostream>
#include "GameConfigLoader.h"
#include "GameRuleEngine.h"
#include "GameInstance.h"

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [game file input]\n";
        return EXIT_FAILURE;
    }

    std::string_view gameFilePath = argv[1];
    // std::cout << "Game file path: " << gameFilePath << "\n";
    // GameConfigLoader configLoader(gameFilePath);

    GameInstance game = GameInstance(gameFilePath);
    game.startGame();

    return EXIT_SUCCESS;

    
}