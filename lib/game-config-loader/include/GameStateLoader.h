#ifndef GAME_STATE_LOADER
#define GAME_STATE_LOADER

#include<map>
#include<string_view>
#include<variant>
#include<vector>
#include <cpp-tree-sitter.h>

using Identifier = std::string_view;
using Primitive = std::variant<int, bool, std::string_view>;
using Map = std::map<std::string_view, Primitive>;
using List = std::vector<std::variant<Primitive, Map>>;
using Value = std::variant<Primitive, Map, List>;
using Environment = std::map<Identifier, Value>;

class GameStateLoader{
private:
    std::string_view source;
    bool isPrimitive(ts::Node node);
    bool isList(ts::Node node);
    bool isMap(ts::Node node);
    Primitive convertNodeToPrimitive(ts::Node node);
    List convertNodeToList(ts::Node node);
    Map convertNodeToMap(ts::Node node);
public:
    GameStateLoader(std::string_view source);
    void printByLevelOrder(const ts::Node& node);
    // Need to pass the root of value_map
    std::unique_ptr<Environment> getEnvironment(ts::Node root);
};
#endif