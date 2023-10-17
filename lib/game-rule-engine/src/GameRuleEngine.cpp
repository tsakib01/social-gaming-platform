#include "GameRuleEngine.h"
#include <iostream>
#include <string.h>

GameRuleEngine& GameRuleEngine::getInstance() {
    static GameRuleEngine instance;
    return instance;
}