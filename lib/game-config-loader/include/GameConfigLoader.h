#ifndef GAME_CONFIG_LOADER_H
#define GAME_CONFIG_LOADER_H

#include <cpp-tree-sitter.h>
#include <string>
#include <memory>
#include <algorithm>
#include "GameState.h"

class GameConfigLoader {
public:
    GameConfigLoader(std::string_view path);
    ~GameConfigLoader() {};
    std::string setSource(std::string_view path);
    std::unique_ptr<ts::Node> transferRules() { return std::move(m_rules); }
    std::unique_ptr<GameState> transferGameState() { return std::move(m_gameState); };

private:
    const std::string m_source;
    std::unique_ptr<ts::Node> m_rules;
    std::unique_ptr<GameState> m_gameState{};

    void loadRules(const ts::Node& root);
    void loadGameState();
    void loadConstants(const ts::Node& root);
};

#endif