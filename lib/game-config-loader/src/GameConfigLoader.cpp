#include "GameConfigLoader.h"
#include "GameRuleEngine.h"
#include <iostream>
#include <fstream>
#include <sstream>

extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}

GameConfigLoader::GameConfigLoader(std::string_view path, std::shared_ptr<GameRuleEngine> gameRuleEngine) 
    : gameRuleEngine(gameRuleEngine), source(setSource(path)) {
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(this->source);
    if(tree.getRootNode().getNumChildren() == 0) {
        throw std::runtime_error("Error: Empty config.");
    }
    ts::Node root = tree.getRootNode();
    this->loadRules(root);
}

std::string GameConfigLoader::setSource(std::string_view path) {
    std::string gameFile(path);
    std::ifstream ifs(gameFile);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    return buffer.str();
}

void GameConfigLoader::loadRules(const ts::Node& root) {
    ts::Node rules = root.getChildByFieldName("rules");
    this->gameRuleEngine->interpretRules(rules, this->source);
}

void GameConfigLoader::printNode(ts::Node node) {
    std::cout <<
        "\ttype - " << node.getType() << "\n" <<
        "\tis named - " << (node.isNamed() ? "yes" : "no") << "\n" <<
        "\tnum children - " << node.getNumChildren() << "\n" <<
        "\tnum named children - " << node.getNumNamedChildren() << "\n";
}

void GameConfigLoader::printTestConfig() {
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    std::string_view input = "[1, null]";
    ts::Tree tree = parser.parseString(input);

    ts::Node root = tree.getRootNode();

    ts::Node array = root.getNamedChild(0);
    ts::Node number = array.getNamedChild(0);

    std::cout << "Root node:\n";
    printNode(root);
    std::cout << "Array node:\n";
    printNode(array);
    std::cout << "Number node:\n";
    printNode(number);

    std::cout << "Whole tree:\n" << root.getSExpr().get() << "\n";
}