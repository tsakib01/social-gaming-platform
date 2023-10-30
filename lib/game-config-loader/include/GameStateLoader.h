#ifndef GAME_STATE_LOADER
#define GAME_STATE_LOADER

#include<map>
#include<string_view>
#include<variant>
#include<vector>
#include "GameEnvironment.h"
#include <cpp-tree-sitter.h>

/**
 * This class takes a root of filed's ts::Node and converts it to GameEnvironment::Environment
 * Currently, it can support variables, constants, per-player and per-audience fields.
*/
class GameStateLoader{
private:
    std::string_view source;
    bool isPrimitive(const ts::Node& node);
    bool isList(const ts::Node& node);
    bool isMap(const ts::Node& node);
    std::unique_ptr<GameEnvironment::Value> convertNodeToPrimitive(const ts::Node& node);
    std::unique_ptr<GameEnvironment::Value> convertNodeToList(const ts::Node& node);
    std::unique_ptr<GameEnvironment::Value> convertNodeToMap(const ts::Node& node);
    std::unique_ptr<GameEnvironment::Value> convertNodeToValue(const ts::Node& node);
public:
    GameStateLoader(std::string_view source);
    void printByLevelOrder(const ts::Node& node);
    // Need to pass the root of value_map
    std::unique_ptr<GameEnvironment::Environment> getEnvironment(const ts::Node& root);
};
#endif