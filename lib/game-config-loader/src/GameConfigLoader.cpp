#include "GameConfigLoader.h"
#include "RuleInterpreter.h"
#include "GameState.h"
#include "GameStateLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include<queue>

extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}

GameConfigLoader::GameConfigLoader(std::string_view path)
: m_source(getSource(path))
{}

std::shared_ptr<GameRules> 
GameConfigLoader::createGameRules() { 
    return std::make_unique<GameRules>(m_source);
}


// TODO: Move handling of loading constants into createGameState

// void GameConfigLoader::loadConstants(const ts::Node& root){
//     ts::Node constants = root.getChildByFieldName("constants");
//     ConstantManager constantManager(constants, source);
//     constantManager.print();
// }


// TODO: Change implementation of handling game state to use std::variant
std::unique_ptr<GameState>
GameConfigLoader::createGameState() {
    auto gameStateLoader = std::make_shared<GameStateLoader>(m_source);
    auto gameState = std::make_unique<GameState>(gameStateLoader);
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    ts::Tree tree = parser.parseString(m_source);

    if(tree.getRootNode().getNumChildren() == 0) {
        throw std::runtime_error("Error: Empty config.");
    }

    ts::Node root = tree.getRootNode();
    gameState->addEnvironment(root.getChildByFieldName("constants").getNamedChild(0));
    gameState->addEnvironment(root.getChildByFieldName("variables").getNamedChild(0));
    return gameState;
}

std::unique_ptr<GameSetup>
createGameSetup(){
//    auto gameSetupLoader = std::make_shared<>()
}

std::string GameConfigLoader::getSource(std::string_view path) {
    std::ifstream ifs(path.data());
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    return buffer.str();
}
