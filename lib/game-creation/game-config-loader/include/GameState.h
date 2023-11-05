#ifndef GAME_STATE
#define GAME_STATE

#include "GameEnvironment.h"
#include <string_view>
#include <variant>
#include <map>

/**
 * This class holds a GameEnvironment::Environment and manges game's state.
*/
class GameState{
public:
    GameState();
    // Add a new environment to the game state.
    void addEnvironment(GameEnvironment::Environment& newEnvironment);
    // Add an identifier with a given value.
    void addState(GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value> value);
    // Get a value of variable by a given identifier
    const GameEnvironment::Value* getValue(GameEnvironment::Identifier identifier);
    // Update the identifier's value to given value.
    void updateState(GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value> value);
    // Print all of identifier and its associative values of the map.
    void print();
private:
    std::unique_ptr<GameEnvironment::Environment> environment;
};
#endif