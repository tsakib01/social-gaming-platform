#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include "RuleInterpreter.h"
#include <stack>
#include <iostream>

class GameInstance {
public:
    GameInstance(std::string_view gameFilePath);
    ~GameInstance() {};
    void startGame();
    void executeNextInstruction();
private:
    std::stack<std::shared_ptr<IRule>> instructionStack;
    std::string source;
};

#endif