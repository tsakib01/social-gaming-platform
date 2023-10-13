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

Expression* GameState::getConstant(std::string key) {
    return constants[key].get();
}

Expression* GameState::getVariable(std::string key) {
    return variables[key].get();
}

Expression* GameState::getPerPlayer(std::string key) {
    return perPlayer[key].get();
}

Expression* GameState::getPerAudience(std::string key) {
    return perAudience[key].get();
}