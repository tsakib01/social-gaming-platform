#include <optional>
#include <functional>
#include <algorithm>
#include "GameState.h"
#include "Expression.h"

void GameState::addConstant(std::string key, std::unique_ptr<Expression> value) {
    constants[key] = std::move(value);
}

void GameState::addVariable(std::string key, std::unique_ptr<Expression> value) {
    variables[key] = std::move(value);
}

void GameState::addPerPlayer(std::string key, std::unique_ptr<Expression> value) {
    perPlayer[key] = std::move(value);
}

void GameState::addPerAudience(std::string key, std::unique_ptr<Expression> value) {
    perAudience[key] = std::move(value);
}

std::unique_ptr<Expression> GameState::getConstant(std::string key) {
    return std::move(constants.at(key));
}

std::unique_ptr<Expression> GameState::getVariable(std::string key) {
    return std::move(variables.at(key));
}

std::unique_ptr<Expression> GameState::getPerPlayer(std::string key) {
    return std::move(perPlayer.at(key));
}


std::unique_ptr<Expression> GameState::getPerAudience(std::string key) {
    return std::move(perAudience.at(key));
}