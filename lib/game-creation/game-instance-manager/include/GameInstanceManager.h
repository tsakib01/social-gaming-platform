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

    // Generates a random unique number between 1000-9999 to be used as a Room Code
    uint16_t generateRoomCode();

    // Creates a GameInstance based off the file path
    void createGameInstance(std::string_view gameFilePath);
    
    // Removes a GameInstance from the list of games 
    void finishGame();

    // Executes the next instruction for each GameInstance
    void runCycle();

    // Returns a list of all Room Codes 
    std::vector<uint16_t> getRoomCodes();

private:
    std::vector<std::unique_ptr<GameInstance>> m_gameList;
};

#endif