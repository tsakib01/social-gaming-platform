#include "GameSetupLoader.h"
#include<string_view>
#include<vector>
#include<memory>
#include <cpp-tree-sitter.h>
class
class GameSetup {
public:
    GameSetup(std::shared_ptr<GameSetupLoader> SetupLoader);
    void addSetup(const ts::Node& node);
    void processGameSetups();
private:
    std::unique_ptr<std::vector<Setup>> setups;
    std::shared_ptr<GameSetupLoader> SetupLoader;
};



