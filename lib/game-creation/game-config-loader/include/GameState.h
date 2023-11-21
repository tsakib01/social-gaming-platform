#ifndef GAME_STATE
#define GAME_STATE

#include "GameEnvironment.h"
#include "Rule.h"
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
    // Get the current execution index of a rule
    size_t getValue(Rule* rule);
    // Update state of a rule
    void updateState(Rule* identifier, size_t value);
    // Print all of identifier and its associative values of the map.
    void print();
private:
    std::unique_ptr<GameEnvironment::Environment> environment;
    std::map<Rule*, size_t> ruleEnvironment;
};
#endif