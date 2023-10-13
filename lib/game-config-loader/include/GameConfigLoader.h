#ifndef GAME_CONFIG_LOADER_H
#define GAME_CONFIG_LOADER_H

#include <cpp-tree-sitter.h>
#include <string>
#include <memory>
#include <algorithm>
#include "GameState.h"
#include "GameRules.h"

class GameConfigLoader {
public:
    // Sets up a GameConfigLoader with a parser, rules and game state pre-loaded
    // @param path Path to a game file
    GameConfigLoader(std::string_view path);
    ~GameConfigLoader() {};
    
    // Loads a new game configuration and stores it within GameConfigLoader
    // @param path Path to the game file
    void loadGameConfig(std::string_view path);

    // Transfers ownership of rules to a GameInstance
    std::unique_ptr<GameRules> transferRules() { return std::move(m_rules); }

    // Transfers ownership of state to a GameInstance
    std::unique_ptr<GameState> transferGameState() { return std::move(m_gameState); };

private:
    std::unique_ptr<GameRules> m_rules{};
    std::unique_ptr<GameState> m_gameState{};

    // Creates the GameRules private member which can be transferred to a GameInstance
    // @param source Game source text
    void loadRules(std::string_view source);

    // TODO: Figure out dependencies for loadGameState
    void loadGameState();

    // TODO: Move handling of loading constants into loadGameState
    // void loadConstants(const ts::Node& root);

    // Reads path to a file and converts to a string
    std::string readSource(std::string_view path);
};

#endif