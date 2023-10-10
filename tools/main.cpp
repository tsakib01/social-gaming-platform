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

    // If parser sees a number expression node, can add to constants map like this:
    gameState.addConstant("testNum", Expression::createNumber(10));
    gameState.addConstant("testString", Expression::createString("helloworld"));

    // Access map entries like this:
    auto constant = gameState.getConstant("testNum");
    if (constant) {
        std::cout << dynamic_cast<NumberExpr*>(constant)->getValue() << '\n';
    }
    constant = gameState.getConstant("testString");
    if (constant) {
        std::cout << dynamic_cast<StringExpr*>(constant)->getValue() << '\n';
    }
    
    return EXIT_SUCCESS;
}