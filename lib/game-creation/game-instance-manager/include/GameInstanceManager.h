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

    // Creates a GameInstance based off the file path
    uint16_t createGameInstance(std::string_view gameFilePath);

    // Moves a GameInstance in gameList to activeGameList 
    // so that it can start executing, and add list of users that join
    void startGame(uint16_t roomCode, const std::vector<User>& users);

    // Executes the next instruction for each GameInstance in m_activeGameList
    void runCycle();

    // Returns a list of all Room Codes 
    std::vector<uint16_t> getRoomCodes();

    void addUsersToGame(uint16_t roomCode, const std::vector<User>& users);
    void deleteUsersFromGame(uint16_t roomCode, const std::vector<User>& users);

private:
    std::vector<std::unique_ptr<GameInstance>> m_gameList;
    std::vector<std::unique_ptr<GameInstance>> m_activeGameList;

    // Generates a random unique number between 1000-9999 to be used as a Room Code
    uint16_t generateRoomCode();
    std::vector<std::unique_ptr<GameInstance>>::iterator getGameItr(uint16_t roomCode); 
};

#endif