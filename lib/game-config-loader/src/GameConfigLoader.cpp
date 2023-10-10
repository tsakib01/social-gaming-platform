#include "GameConfigLoader.h"
#include "RuleInterpreter.h"
#include "ConstantManager.h"
#include "GameState.h"
#include <iostream>
#include <fstream>
#include <sstream>

extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}

GameConfigLoader::GameConfigLoader(std::string_view path) : m_source(setSource(path)) {
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(m_source);
    if(tree.getRootNode().getNumChildren() == 0) {
        throw std::runtime_error("Error: Empty config.");
    }
    ts::Node root = tree.getRootNode();
    loadConstants(root);
    loadRules(root);
    loadGameState();
}

std::string GameConfigLoader::setSource(std::string_view path) {
    std::ifstream ifs(path.data());
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    return buffer.str();
}

void GameConfigLoader::loadRules(const ts::Node& root) {
    std::unique_ptr<ts::Node> rules = std::make_unique<ts::Node>(root.getChildByFieldName("rules"));
    m_rules = std::move(rules);
}

void GameConfigLoader::loadConstants(const ts::Node& root){
    ts::Node constants = root.getChildByFieldName("constants");
    ConstantManager constantManager(constants, m_source);
    constantManager.print();
}

void GameConfigLoader::loadGameState() {
    m_gameState = std::make_unique<GameState>();

    // If parser sees a number expression node, can add to constants map like this:
    m_gameState->addConstant("testNum", Expression::createNumber(10));
    m_gameState->addConstant("testString", Expression::createString("helloworld"));

    // Access map entries like this:
    auto constant = m_gameState->getConstant("testNum");
    if (constant) {
        std::cout << dynamic_cast<NumberExpr*>(constant)->getValue() << '\n';
    }
    constant = m_gameState->getConstant("testString");
    if (constant) {
        std::cout << dynamic_cast<StringExpr*>(constant)->getValue() << '\n';
    }
}