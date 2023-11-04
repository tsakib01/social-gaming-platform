#ifndef GAME_CONFIG_LOADER_H
#define GAME_CONFIG_LOADER_H

#include <cpp-tree-sitter.h>
#include <string>
#include <memory>
#include <algorithm>
#include "GameState.h"
#include "GameSetup.h"
#include "Translator.h"
#include <iostream>

class GameConfigLoader {
public:
    GameConfigLoader(std::string_view path);

    // Creates a GameRules for a GameInstance 
    // @param path Path to a game file 
    [[nodiscard]] std::unique_ptr<RuleTree> 
    createGameRules();

    // Creates a GameState for a GameInstance
    // Transfers ownership of state to a GameInstance
    [[nodiscard]] std::unique_ptr<GameState> 
    createGameState();
    [[nodiscard]] std::unique_ptr<GameSetup>
    createGameSetup();
private:
    // TODO: Move handling of loading constants into loadGameState
    // void loadConstants(const ts::Node& root);
    std::string m_source;

    std::string getSource(std::string_view path);
};

#endif