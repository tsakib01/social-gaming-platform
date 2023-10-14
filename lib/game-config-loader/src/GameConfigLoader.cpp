#include "GameConfigLoader.h"
#include "RuleInterpreter.h"
#include "GameState.h"
#include "GameStateLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include<queue>


std::shared_ptr<GameRules> 
GameConfigLoader::createGameRules(std::string_view path) { 
    setSource(path);
    return std::make_unique<GameRules>(m_source);
}


// TODO: Move handling of loading constants into createGameState

// void GameConfigLoader::loadConstants(const ts::Node& root){
//     ts::Node constants = root.getChildByFieldName("constants");
//     ConstantManager constantManager(constants, source);
//     constantManager.print();
// }


// TODO: Change implementation of handling game state to use std::variant
std::shared_ptr<GameState>
GameConfigLoader::createGameState() {
    auto gameState = std::make_shared<GameState>();

    // If parser sees a number expression node, can add to constants map like this:
    gameState->addConstant("testNum", Expression::createNumber(10));
    gameState->addConstant("testString", Expression::createString("helloworld"));

    // Access map entries like this:
    auto constant = gameState->getConstant("testNum");
    if (constant) {
        std::cout << dynamic_cast<IntExpression*>(constant)->getValue() << '\n';
    }
    constant = gameState->getConstant("testString");
    if (constant) {
        std::cout << dynamic_cast<StringExpression*>(constant)->getValue() << '\n';
    }

    return gameState;
}


void 
GameConfigLoader::setSource(std::string_view path) {
    if (path == m_path) {
        return;
    }

    m_path = path;

    std::ifstream ifs(path.data());
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    m_source = buffer.str();
}