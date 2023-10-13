#include "GameState.h"
#include <iostream>

GameState::GameState(std::shared_ptr<GameStateLoader> gameStateLoader)
:environment(std::make_unique<Environment>()), gameStateLoader(gameStateLoader)
{}

void GameState::addEnvironment(ts::Node root){
    std::unique_ptr<Environment> newEnvironment = gameStateLoader->getEnvironment(root);

    // Check there are no same identifiers
    for (const auto& [identifier, value] : *newEnvironment){
        if (environment->count(identifier)){
            std::runtime_error("Top-level identifier should be same.");
        }
    }
    
    // Insert variables to environment
    for (const auto& [identifier, value] : *newEnvironment){
        environment->insert(std::make_pair(identifier, value));
    }
}

void GameState::addState(Identifier identifier, Value value){
    if (environment->count(identifier)){
        std::runtime_error ("The identifier already exists in the environment");
    }
    environment->insert(std::make_pair(identifier, value));
}

void GameState::updateState(Identifier identifier, Value value){
    if (environment->count(identifier)){
        std::runtime_error ("The identifier does not exists in the environment");
    }
    environment->insert(std::make_pair(identifier, value));
}

struct PrintVisitor {
    void operator()(const Primitive& value) const {
        if (std::holds_alternative<int>(value)) {
            std::cout << std::get<int>(value);
        } else if (std::holds_alternative<bool>(value)) {
            std::cout << (std::get<bool>(value) ? "true" : "false");
        } else if (std::holds_alternative<std::string_view>(value)) {
            std::cout << std::get<std::string_view>(value);
        }
    }

    void operator()(const Map& value) const {
        std::cout << "{ ";
        for (const auto& [key, val] : value) {
            std::cout << key << ": ";
            std::visit(*this, val); // Recursively visit the map values.
            std::cout << ", ";
        }
        std::cout << "} ";
    }

    void operator()(const List& value) const {
        std::cout << "[ ";
        for (const auto& item : value) {
            std::visit(*this, item); // Recursively visit the list items.
            std::cout << ", ";
        }
        std::cout << "] ";
    }

    template <typename T>
    void operator()(const T& value) const {
        std::cout << value;
    }

    void operator()(const Value& value) const {
        // Use std::visit to visit the variant type and delegate to appropriate functions.
        std::visit(*this, value);
    }
};

void GameState::print(){
    for (const auto& [key, value] : *environment){
        std::cout << key << ": ";
        std::visit(PrintVisitor{}, value);
        std::cout << std::endl;
    }
}



