#include "GameConfigLoader.h"
#include "RuleInterpreter.h"
#include "ConstantManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}


GameConfigLoader::GameConfigLoader(std::string_view path) { 
    loadGameConfig(path);
}


void GameConfigLoader::loadGameConfig(std::string_view path) {
    std::string source = readSource(path);
    loadRules(source);
    loadGameState();
}   


void GameConfigLoader::loadRules(std::string_view source) { 
    m_rules = std::make_unique<GameRules>(source);
}


// TODO: Move handling of loading constants into loadGameState

// void GameConfigLoader::loadConstants(const ts::Node& root){
//     ts::Node constants = root.getChildByFieldName("constants");
//     ConstantManager constantManager(constants, source);
//     constantManager.print();
// }


// TODO: Change implementation of handling game state to use std::variant
void GameConfigLoader::loadGameState() {
    m_gameState = std::make_unique<GameState>();

    // If parser sees a number expression node, can add to constants map like this:
    m_gameState->addConstant("testNum", Expression::createNumber(10));
    m_gameState->addConstant("testString", Expression::createString("helloworld"));

    // Access map entries like this:
    auto constant = m_gameState->getConstant("testNum");
    if (constant) {
        std::cout << dynamic_cast<IntExpression*>(constant)->getValue() << '\n';
    }
    constant = m_gameState->getConstant("testString");
    if (constant) {
        std::cout << dynamic_cast<StringExpression*>(constant)->getValue() << '\n';
    }
}


std::string GameConfigLoader::readSource(std::string_view path) {
    std::ifstream ifs(path.data());
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    return buffer.str();
}