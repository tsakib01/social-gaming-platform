#ifndef GAME_CONFIG_LOADER_H
#define GAME_CONFIG_LOADER_H

#include <cpp-tree-sitter.h>
#include <string>

class GameConfigLoader {
public:
    GameConfigLoader(std::string_view path);
    ~GameConfigLoader() {};
    std::string setSource(std::string_view path);
    void loadRules(const ts::Node& root);
    void loadConstants(const ts::Node& root);
private:
    const std::string source;
};

#endif