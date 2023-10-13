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

GameConfigLoader::GameConfigLoader(std::string_view path) : source(setSource(path)) {
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(this->source);
    if(tree.getRootNode().getNumChildren() == 0) {
        throw std::runtime_error("Error: Empty config.");
    }
    ts::Node root = tree.getRootNode();
    std::shared_ptr<GameStateLoader> gameStateLoader = std::make_shared<GameStateLoader>(source);
    std::unique_ptr<GameState> gameState = std::make_unique<GameState>(gameStateLoader);
    gameState->addEnvironment(root.getChildByFieldName("constants").getNamedChild(0));
    gameState->addEnvironment(root.getChildByFieldName("variables").getNamedChild(0));
    gameState->print();
    this->loadRules(root);
}

std::string GameConfigLoader::setSource(std::string_view path) {
    std::ifstream ifs(path.data());
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    return buffer.str();
}

void GameConfigLoader::loadRules(const ts::Node& root) {
    ts::Node rules = root.getChildByFieldName("rules");
    RuleInterpreter::interpretRules(rules, this->source);
}