#include <optional>
#include <functional>
#include "GameState.h"
#include "Expression.h"

void GameState::addConstant(std::string key, Expression value) {
    constants[key] = value;
}

void GameState::addVariable(std::string key, Expression value) {
    variables[key] = value;
}

void GameState::addPerPlayer(std::string key, Expression value) {
    perPlayer[key] = value;
}

void GameState::addPerAudience(std::string key, Expression value) {
    perAudience[key] = value;
}

Expression& GameState::getConstant(std::string key) {
    return constants[key];
}

Expression& GameState::getVariable(std::string key) {
    return variables[key];
}

Expression& GameState::getPerPlayer(std::string key) {
    return perPlayer[key];
}

Expression& GameState::getPerAudience(std::string key) {
    return perAudience[key];
}