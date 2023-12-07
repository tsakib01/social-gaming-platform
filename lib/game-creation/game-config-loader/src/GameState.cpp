#include "GameState.h"
#include <iostream>

GameState::GameState()
:environment(std::make_unique<GameEnvironment::Environment>())
{}

void GameState::addEnvironment(GameEnvironment::Environment& newEnvironment){
    // Insert variables to environment
    for (auto& [identifier, value] : newEnvironment){
        auto [it, succeeded] = environment->try_emplace(identifier, std::move(value));
        if (!succeeded){
            std::runtime_error("Top-level identifier should be same.");
        }
    }
}

void GameState::addState(GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value> value){
    auto [it, succeeded]  = environment->try_emplace(identifier, std::move(value));
    if (!succeeded){
        std::runtime_error ("The identifier already exists in the environment");
    }
}

void GameState::addSetupToGameState(GameEnvironment::Identifier identifier,  GameEnvironment::Value toStore){
    auto configuration = environment->find("configuration");
    if(configuration==environment->end()){
        throw std::runtime_error ("No configuration in GameState");
    }
    auto identifierValue = GameEnvironment::Value(identifier);

    std::vector<GameEnvironment::Value*> values={configuration->second.get(), &identifierValue, &toStore};
    evaluator.evaluate(MODIFIER::SET, values);
}

GameEnvironment::Value GameState::getValue(GameEnvironment::Identifier identifier) const {
    auto variable = environment->find(identifier);
    if (variable == environment->end()) {
        std::runtime_error ("The identifier does not exists in the environment");
    }
    return GameEnvironment::Value(*(variable->second));
}

void GameState::updateState(GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value>  value){
    auto variable = environment->find(identifier);
    if (variable == environment->end()){
        std::runtime_error ("The identifier does not exists in the environment");
    }
    variable->second = std::move(value);
}

bool GameState::hasValue(GameEnvironment::Identifier identifier) {
    auto variable = environment->find(identifier);
    if (variable == environment->end()){
        return false;
    }
    return true;
}

// struct PrintVisitor {
//     void operator()(int value) const {
//         std::cout << "Integer: " << value << std::endl;
//     }

//     void operator()(bool value) const {
//         std::cout << "Boolean: " << (value ? "true" : "false") << std::endl;
//     }

//     void operator()(const std::string_view& value) const {
//         std::cout << "String: " << value << std::endl;
//     }

//     void operator()(const std::unique_ptr<GameEnvironment::Map>& value) const {
//         std::cout << "Map found..." << std::endl;
//         for (const auto& [key, val] : *value) {
//             std::cout << key << ": ";
//             std::visit(PrintVisitor{}, val->value);
//             std::cout << std::endl;
//         }
//     }

//     void operator()(const std::unique_ptr<GameEnvironment::List>& value) const {
//         std::cout << "List found..." << std::endl;
//         for (const auto& item : *value) {
//             std::visit(PrintVisitor{}, item->value);
//         }
//     }

//     void operator()(const GameEnvironment::Value& value) const {
//         std::visit(*this, value.value);
//     }
// };

size_t GameState::getValue(Rule* rule) {
    auto variable = ruleEnvironment.find(rule);
    if (variable == ruleEnvironment.end()) {
        ruleEnvironment[rule] = 0;
        return 0;
    }
    return variable->second;
}

void GameState::updateState(Rule* rule, size_t value) {
    auto it = ruleEnvironment.find(rule);
    if (it == ruleEnvironment.end()) {
        throw std::runtime_error ("The rule does not exists in the environment");
    }
    ruleEnvironment[rule] = value;
}

void GameState::removeValue(Rule* rule) {
    auto it = ruleEnvironment.find(rule);
    if (it == ruleEnvironment.end()) {
        throw std::runtime_error ("The rule does not exists in the environment");
    }
    ruleEnvironment.erase(it);
}

void GameState::addPlayerState(const User& user) {
    if (!hasValue("players")) {
        std::unique_ptr<GameEnvironment::List> playerList = std::make_unique<GameEnvironment::List>();
        addState("players", std::make_unique<GameEnvironment::Value>(std::move(playerList)));
        numPlayers = 0;
    }
    auto userObject = std::make_unique<GameEnvironment::Value>((getValue("per-player")));
    GameEnvironment::Value nameIdentifier(std::string_view("name"));
    GameEnvironment::Value nameToSet(user.username);
    std::vector<GameEnvironment::Value*> nameValues={userObject.get(), &nameIdentifier, &nameToSet};
    GameEnvironment::Value idIdentifier(std::string_view("id"));
    GameEnvironment::Value idToSet((int)user.userID.id);
    std::vector<GameEnvironment::Value*> idValues={userObject.get(), &idIdentifier, &idToSet};
    evaluator.evaluate(MODIFIER::SET, nameValues);
    evaluator.evaluate(MODIFIER::SET, idValues);
    auto players = environment->find("players");
    GameEnvironment::Value index(numPlayers);
    std::vector<GameEnvironment::Value*> playerValues={players->second.get(), &index, userObject.get()};
    evaluator.evaluate(MODIFIER::SET, playerValues);
    numPlayers++;
}

void GameState::print(){
    for (const auto& [key, value] : *environment){
        std::cout << key << ": ";
        std::visit(PrintVisitor{}, value->value);
        std::cout << std::endl;
    }
}




