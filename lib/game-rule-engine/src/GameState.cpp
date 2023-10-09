#include <optional>
#include <functional>
#include "GameState.h"
#include "Expression.h"

void GameState::addConstant(std::string key, Expression* value) {
    constants[key] = value;
}

void GameState::addVariable(std::string key, Expression* value) {
    variables[key] = value;
}

void GameState::addPerPlayer(std::string key, Expression* value) {
    perPlayer[key] = value;
}

void GameState::addPerAudience(std::string key, Expression* value) {
    perAudience[key] = value;
}

std::optional<std::reference_wrapper<Expression>> GameState::getConstant(std::string key) {
    auto constant{constants.find(key)};
    if (constant == constants.end()) {
        return std::nullopt;
    }
    return *(constant->second);
}

std::optional<std::reference_wrapper<Expression>> GameState::getVariable(std::string key) {
    return std::nullopt;
}

std::optional<std::reference_wrapper<Expression>> GameState::getPerPlayer(std::string key) {
    return std::nullopt;
}

std::optional<std::reference_wrapper<Expression>> GameState::getPerAudience(std::string key) {
    return std::nullopt;
}