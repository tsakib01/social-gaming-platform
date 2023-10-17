#ifndef GAME_SETUP
#define GAME_SETUP
#include "GameSetupLoader.h"
#include<string_view>
#include<vector>
#include<memory>
#include <cpp-tree-sitter.h>

class GameSetup {
public:
    GameSetup(std::shared_ptr<GameSetupLoader> SetupLoader);
    void addSetups(ts::Node node);
    void processGameSetups();
private:
    std::unique_ptr<std::vector<Setup>> setups;
    std::shared_ptr<GameSetupLoader> SetupLoader;
};
#endif


