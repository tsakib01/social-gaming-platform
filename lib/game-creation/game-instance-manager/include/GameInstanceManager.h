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
    
    uint16_t createGameInstance(std::string_view gameFilePath);
    
    void startGame(uint16_t roomCode, const std::vector<User>& users);
    void deleteGame(uint16_t roomCode);

    ConfigResult inputConfig(uint16_t roomCode, const std::string& response);
    void runCycle();
    
    std::vector<uint16_t> getRoomCodes();
    
    void addUsersToGame(uint16_t roomCode, const std::vector<User>& users);
    void deleteUsersFromGame(uint16_t roomCode, const std::vector<User>& users);
    bool gameIsJoinable(uint16_t roomCode);
    bool gameHasSetup(uint16_t roomCode);

private:
    std::vector<std::unique_ptr<GameInstance>> m_gameList;

    // Generates a random unique number between 1000-9999
    uint16_t generateRoomCode();
    std::unique_ptr<GameInstance>& getGameInstance(uint16_t roomCode); 
};

#endif