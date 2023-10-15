#ifndef GAME_INSTANCE_MANAGER_H
#define GAME_INSTANCE_MANAGER_H

#include <iostream>
#include <vector>
#include <random>
#include "GameInstance.h"
#include "GameConfigLoader.h"

class GameInstanceManager {
public:
    GameInstanceManager();
    ~GameInstanceManager() {};

    int generateInviteCode();
    void sendInviteCode();
    void createGameInstance(std::string_view gameFilePath);

    void addGameToActiveList();
    void removeGameFromActiveList();

    void runCycle();

private:
    std::vector<std::unique_ptr<GameInstance>> m_activeGameList;
    std::vector<std::unique_ptr<GameInstance>> m_gameList;
};

#endif