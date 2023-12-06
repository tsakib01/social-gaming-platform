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
    std::vector<std::string_view> getIdentifiers();
    std::vector<std::string_view> getPrompts();
    std::vector<std::string_view> getRestInfos();
    bool isResponseValid(std::string_view identifier,std::string_view response);
    KIND getKind(std::string_view identifier) const;
    bool hasSetup();
private:
    std::vector<std::unique_ptr<SetupInstance>> setups;
    std::shared_ptr<GameSetupLoader> SetupLoader;
};
#endif


