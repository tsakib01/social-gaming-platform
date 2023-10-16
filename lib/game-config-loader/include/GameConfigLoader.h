#ifndef GAME_CONFIG_LOADER_H
#define GAME_CONFIG_LOADER_H

#include <cpp-tree-sitter.h>
#include <string>
#include <memory>
#include <algorithm>
#include "GameState.h"
#include "GameRules.h"
#include <iostream>

class GameConfigLoader {
public:
    GameConfigLoader(std::string_view path);

    // Creates a GameRules for a GameInstance 
    // @param path Path to a game file 
    [[nodiscard]] std::shared_ptr<GameRules> 
    createGameRules();

    // Creates a GameState for a GameInstance
    // Transfers ownership of state to a GameInstance
    [[nodiscard]] std::unique_ptr<GameState> 
    createGameState(); 
private:
    // TODO: Move handling of loading constants into loadGameState
    // void loadConstants(const ts::Node& root);
    std::string m_source;

    std::string getSource(std::string_view path);
};

#endif