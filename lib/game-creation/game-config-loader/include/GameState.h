#ifndef GAME_STATE
#define GAME_STATE

#include "GameEnvironment.h"
#include "Rule.h"
#include <string_view>
#include <iostream>
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
    // Add a setup to game state
    void addSetupToGameState(GameEnvironment::Identifier identifier,  GameEnvironment::Value value);
    // Get a value of variable by a given identifier
    GameEnvironment::Value getValue(GameEnvironment::Identifier identifier);
    // Update the identifier's value to given value.
    void updateState(GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value> value);
    // Get the current execution index of a rule
    size_t getValue(Rule* rule);
    // Update state of a rule
    void updateState(Rule* identifier, size_t value);
    // Removes rule from ruleEnvironment
    void removeValue(Rule* identifier);
    // Print all of identifier and its associative values of the map.
    void print();
private:
    std::unique_ptr<GameEnvironment::Environment> environment;
    std::map<Rule*, size_t> ruleEnvironment;
};

struct PrintVisitor {
    void operator()(int value) const {
        std::cout << "Integer: " << value << std::endl;
    }

    void operator()(bool value) const {
        std::cout << "Boolean: " << (value ? "true" : "false") << std::endl;
    }

    void operator()(const std::string_view& value) const {
        std::cout << "String: " << value << std::endl;
    }

    void operator()(const std::unique_ptr<GameEnvironment::Map>& value) const {
        std::cout << "Map found..." << std::endl;
        for (const auto& [key, val] : *value) {
            std::cout << key << ": ";
            std::visit(PrintVisitor{}, val->value);
            std::cout << std::endl;
        }
    }

    void operator()(const std::unique_ptr<GameEnvironment::List>& value) const {
        std::cout << "List found..." << std::endl;
        for (const auto& item : *value) {
            std::visit(PrintVisitor{}, item->value);
        }
    }

    void operator()(const GameEnvironment::Value& value) const {
        std::visit(*this, value.value);
    }
};




#endif