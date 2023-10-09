#include "GameConfigLoader.h"
#include "RuleInterpreter.h"
#include "ConstantManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
    this->loadConstants(root);
    this->loadRules(root);
}

std::string GameConfigLoader::setSource(std::string_view path) {
    std::ifstream ifs(path.data());
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    return buffer.str();
}

ts::Node GameConfigLoader::loadRules(const ts::Node& root) {
    return root.getChildByFieldName("rules");
}

void GameConfigLoader::loadConstants(const ts::Node& root){
    ts::Node constants = root.getChildByFieldName("constants");
    ConstantManager constantManager(constants, this->source);
    constantManager.print();
}