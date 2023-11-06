#include "GameConfigLoader.h"
#include "GameState.h"
#include "GameStateLoader.h"
#include "GameSetup.h"
#include "GameSetupLoader.h"
#include "Translator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}

GameConfigLoader::GameConfigLoader(std::string_view path)
: m_source(getSource(path))
{}

std::unique_ptr<RuleTree> 
GameConfigLoader::createGameRules() { 
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(m_source);
    ts::Node root = tree.getRootNode();
    ts::Node rootBodyNode = root.getChildByFieldName("rules").getNamedChild(0);
    
    Translator translator{createTranslator(m_source)};
    return translator.translate(rootBodyNode);
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
GameConfigLoader::createGameSetup(){

    auto gameSetupLoader = std::make_shared<GameSetupLoader>(m_source);
    auto gameSetup = std::make_unique<GameSetup>(gameSetupLoader);
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    ts::Tree tree = parser.parseString(m_source);

    if(tree.getRootNode().getNumChildren() == 0) {
        throw std::runtime_error("Error: Empty config.");
    }
    ts::Node root = tree.getRootNode();
    gameSetup->addSetups(root.getChildByFieldName("configuration"));
    return gameSetup;
}

std::string GameConfigLoader::getSource(std::string_view path) {
    std::ifstream ifs(path.data());
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    return buffer.str();
}
