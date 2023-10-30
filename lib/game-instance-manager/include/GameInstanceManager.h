#ifndef GAME_INSTANCE_MANAGER_H
#define GAME_INSTANCE_MANAGER_H

#include <iostream>
#include <vector>
#include <random>
#include "GameInstance.h"
#include "GameConfigLoader.h"
#include "UserManager.h"

class GameInstanceManager {
public:
    GameInstanceManager();
    ~GameInstanceManager() {};

    uint16_t generateRoomCode();
    void createGameInstance(std::string_view gameFilePath);

    void startGame();
    void finishGame();
    void runCycle();
    
    void insertUsersIntoGame(std::vector<std::shared_ptr<User>> users, uint16_t roomCode);

private:
    std::vector<std::unique_ptr<GameInstance>> m_activeGameList;
    std::vector<std::unique_ptr<GameInstance>> m_gameList;
};

#endif