#include "GameState.h"
#include <iostream>

GameState::GameState()
:environment(std::make_unique<GameEnvironment::Environment>())
{}

void GameState::addEnvironment(std::unique_ptr<GameEnvironment::Environment> newEnvironment){
    // Check there are no same identifiers
    for (const auto& [identifier, value] : *newEnvironment){
        if (environment->count(identifier)){
            std::runtime_error("Top-level identifier should be same.");
        }
    }
    
    // Insert variables to environment
    for (auto& [identifier, value] : *newEnvironment){
        environment->insert(std::make_pair(identifier, std::move(value)));
    }
}

void GameState::addState(GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value> value){
    if (environment->count(identifier)){
        std::runtime_error ("The identifier already exists in the environment");
    }
    environment->insert(std::make_pair(identifier,  std::move(value)));
}

void GameState::updateState(GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value>  value){
    if (environment->count(identifier)){
        std::runtime_error ("The identifier does not exists in the environment");
    }
    environment->insert(std::make_pair(identifier,  std::move(value)));
}

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
        for (const auto& [key, val] : value->value) {
            std::cout << key << ": ";
            std::visit(PrintVisitor{}, val->value);
            std::cout << std::endl;
        }
    }

    void operator()(const std::unique_ptr<GameEnvironment::List>& value) const {
        std::cout << "List found..." << std::endl;
        for (const auto& item : value->value) {
            std::visit(PrintVisitor{}, item->value);
        }
    }

    void operator()(const GameEnvironment::Value& value) const {
        std::visit(*this, value.value);
    }
};
    
void GameState::print(){
    for (const auto& [key, value] : *environment){
        std::cout << key << ": ";
        std::visit(PrintVisitor{}, value->value);
        std::cout << std::endl;
    }
}




