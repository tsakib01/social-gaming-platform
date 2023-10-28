#ifndef GAME_STATE
#define GAME_STATE

#include "GameEnvironment.h"
#include <string_view>
#include <variant>
#include <map>

class GameState{
public:
    GameState();
    void addEnvironment(std::unique_ptr<GameEnvironment::Environment> newEnvironment);
    void addState(GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value> value);
    void updateState(GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value> value);
    //GameEnvironment::Value& getValue(GameEnvironment::Identifier identifier);
    void print();
private:
    std::unique_ptr<GameEnvironment::Environment> environment;
};
#endif