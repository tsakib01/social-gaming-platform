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
    // Creates a GameRules for a GameInstance 
    // @param path Path to a game file 
    [[nodiscard]] std::shared_ptr<GameRules> 
    createGameRules(std::string_view path);

    // Creates a GameState for a GameInstance
    // Transfers ownership of state to a GameInstance
    [[nodiscard]] std::shared_ptr<GameState> 
    createGameState(); 

private:
    // Cache source text to avoid multiple file IOs
    std::string m_source{};
    std::string m_path{};

    // TODO: Move handling of loading constants into loadGameState
    // void loadConstants(const ts::Node& root);

    // Sets and caches game source text if path is different from last path
    // @param path Path to a game file
    void setSource(std::string_view path);
};

#endif