#ifndef GAME_CONFIG_LOADER_H
#define GAME_CONFIG_LOADER_H

#include "GameRuleEngine.h"
#include <cpp-tree-sitter.h>
#include <string>

class GameConfigLoader {
public:
    GameConfigLoader(std::string_view path, std::shared_ptr<GameRuleEngine> gameRuleEngine);
    ~GameConfigLoader() {};
    void printTestConfig();
    std::string setSource(std::string_view path);
    void loadRules(const ts::Node& root);
    void printNode(ts::Node root);
private:
    std::shared_ptr<GameRuleEngine> gameRuleEngine;
    const std::string source;
};

#endif