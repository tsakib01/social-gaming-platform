#ifndef GAME_STATE_LOADER
#define GAME_STATE_LOADER

#include<map>
#include<string_view>
#include<variant>
#include<vector>
#include "GameEnvironment.h"
#include "GameSetupLoader.h"
#include <cpp-tree-sitter.h>

class GameStateLoader;
class ConvertInterface {
public:
    ConvertInterface(const GameStateLoader* gameStateLoader)
    : gameStateLoader(gameStateLoader)
    {}

    std::unique_ptr<GameEnvironment::Value> convertNode(const ts::Node& node){
        return convertNodeImpl(node);
    }

    virtual ~ConvertInterface() = default;
protected:
    const GameStateLoader* gameStateLoader;
private:
    virtual std::unique_ptr<GameEnvironment::Value> convertNodeImpl(const ts::Node& node) const = 0;
};

/**
 * This class takes a root of filed's ts::Node and converts it to GameEnvironment::Environment
 * Currently, it can support variables, constants, per-player and per-audience fields.
*/
class GameStateLoader{
private:
    std::string_view source;
    std::map<int, std::unique_ptr<ConvertInterface>> nodeSymbolToConvert;
public:
    GameStateLoader() = default;
    GameStateLoader(std::string_view source);
    void printByLevelOrder(const ts::Node& node);
    void registerConversion(int symbol, std::unique_ptr<ConvertInterface> convert);
    std::string_view getSource() const;
    const std::map<int, std::unique_ptr<ConvertInterface>>* getNodeSymbolToConvert() const;
    std::unique_ptr<GameEnvironment::Value> convertNode(const ts::Node& node) const;
    // // Need to pass the root of value_map
    std::unique_ptr<GameEnvironment::Environment> getEnvironment(const ts::Node& root);
    std::unique_ptr<GameEnvironment::Environment> getConfigEnvironment(const ts::Node& root);
    std::unique_ptr<GameEnvironment::Environment> getPer_playerEnvironment(const ts::Node& root);
    static std::unique_ptr<GameStateLoader> createDefaultGameStateLoader(std::string_view source);
};
#endif