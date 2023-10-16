#ifndef GAME_STATE
#define GAME_STATE

#include "GameStateLoader.h"
#include <string_view>
#include <variant>
#include <map>

class GameState{
public:
    GameState(std::shared_ptr<GameStateLoader> gameStateLoader);
    void addEnvironment(ts::Node root);
    void addState(Identifier identifier, Value value);
    void updateState(Identifier identifier, Value value);
    Value getData(Identifier identifier);
    void print();
private:
    std::unique_ptr<Environment> environment;
    std::shared_ptr<GameStateLoader> gameStateLoader;
};
#endif