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

void GameState::addSetupToGameState(GameEnvironment::Identifier identifier,  std::unique_ptr<GameEnvironment::Value> value){
    if(environment->find("configuration")==environment->end()){
        throw std::runtime_error ("No configuration in GameState");
    }
    auto configuration = environment->find("configuration");
    EmplaceVisitor visitor(identifier,std::move(value));
    std::visit(visitor, configuration->second->value);

//    configuration->second->value.emplace(identifier, std::move(value));
}
const GameEnvironment::Value* GameState::getValue(GameEnvironment::Identifier identifier){
    auto variable = environment->find(identifier);
    if (variable == environment->end()){
        std::runtime_error ("The identifier does not exists in the environment");
    }
    return variable->second.get();
}

void GameState::updateState(GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value>  value){
    auto variable = environment->find(identifier);
    if (variable == environment->end()){
        std::runtime_error ("The identifier does not exists in the environment");
    }
    variable->second = std::move(value);
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

void GameState::print(){
    for (const auto& [key, value] : *environment){
        std::cout << key << ": ";
        std::visit(PrintVisitor{}, value->value);
        std::cout << std::endl;
    }
}




