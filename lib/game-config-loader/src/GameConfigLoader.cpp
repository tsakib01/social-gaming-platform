#include "GameConfigLoader.h"
#include "RuleInterpreter.h"
#include "ConstantManager.h"
#include "GameState.h"
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
    this->loadRules(root);
    this->loadConstants(root);
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

// Print node by level order
void GameConfigLoader::printByLevelOrder(const ts::Node& node){
    std::queue<ts::Node> nodeQueue;
    nodeQueue.push(node);

    while (!nodeQueue.empty()){
        int queueSize = nodeQueue.size();

        for (int i = 0; i < queueSize; i++){
            ts::Node node = nodeQueue.front();
            nodeQueue.pop();
            std::cout << node.getType() << ", ";
            for (uint32_t j = 0; j < node.getNumNamedChildren(); j++){
                nodeQueue.push(node.getNamedChild(j));
            }
        }
        std::cout << "\n";
    }
}

void GameConfigLoader::loadConstants(const ts::Node& root){
    ts::Node constants = root.getChildByFieldName("constants");
    printByLevelOrder(constants);
    GameState gameSate(constants.getSourceRange(source));
}