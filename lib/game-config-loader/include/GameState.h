#ifndef GAME__STATE__H
#define GAME__STATE__H

#include <unordered_map>
#include <optional>
#include <functional>
#include <string>
#include <memory>
#include "Expression.h"

class GameState {
public:
    GameState() {};
    void addConstant(std::string key, std::unique_ptr<Expression> value);
    void addVariable(std::string key, std::unique_ptr<Expression> value);
    void addPerPlayer(std::string key, std::unique_ptr<Expression> value);
    void addPerAudience(std::string key, std::unique_ptr<Expression> value);
    std::unique_ptr<Expression> getConstant(std::string key);
    std::unique_ptr<Expression> getVariable(std::string key);
    std::unique_ptr<Expression> getPerPlayer(std::string key);
    std::unique_ptr<Expression> getPerAudience(std::string key);

private:
    std::unordered_map<std::string, std::unique_ptr<Expression>> constants;
    std::unordered_map<std::string, std::unique_ptr<Expression>> variables;
    std::unordered_map<std::string, std::unique_ptr<Expression>> perPlayer;
    std::unordered_map<std::string, std::unique_ptr<Expression>> perAudience;
};

#endif