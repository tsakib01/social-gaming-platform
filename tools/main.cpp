#include <iostream>
#include "GameConfigLoader.h"
#include "GameRuleEngine.h"
#include "GameState.h"
#include "Expression.h"

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [game file input]\n";
        return EXIT_FAILURE;
    }

    std::string_view gameFilePath = argv[1];
    std::cout << "Game file path: " << gameFilePath << "\n";
    GameConfigLoader configLoader(gameFilePath);
    GameState gameState{};
    NumberExpr test{10};
    gameState.addConstant("test", &test);
    auto constant = gameState.getConstant("test");
    if (constant) {
        std::cout << dynamic_cast<NumberExpr*>(constant)->getValue() << '\n';
    }
    
    return EXIT_SUCCESS;
}