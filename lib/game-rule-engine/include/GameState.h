#ifndef GAME__STATE__H
#define GAME__STATE__H

#include <unordered_map>
#include <optional>
#include <functional>
#include <string>
#include "Expression.h"

class GameState {
public:
    GameState() {};
    void addConstant(std::string key, Expression* value);
    void addVariable(std::string key, Expression* value);
    void addPerPlayer(std::string key, Expression* value);
    void addPerAudience(std::string key, Expression* value);
    std::optional<std::reference_wrapper<Expression>> getConstant(std::string key);
    std::optional<std::reference_wrapper<Expression>> getVariable(std::string key);
    std::optional<std::reference_wrapper<Expression>> getPerPlayer(std::string key);
    std::optional<std::reference_wrapper<Expression>> getPerAudience(std::string key);

private:
    std::unordered_map<std::string, Expression*> constants;
    std::unordered_map<std::string, Expression*> variables;
    std::unordered_map<std::string, Expression*> perPlayer;
    std::unordered_map<std::string, Expression*> perAudience;
};

#endif