#ifndef GAME_INSTANCE_MANAGER_H
#define GAME_INSTANCE_MANAGER_H

#include <iostream>
#include <vector>

class GameInstanceManager {
public:
    GameInstanceManager();
    ~GameInstanceManager() {};
    
    std::string_view sendGameList();
    void sendGameConfiguration(std::string_view path);
    void sendInviteCode();
    void createGameInstance();

    void addGameToActiveList();
    void removeGameFromActiveList();

    void addPlayer();
    void removePlayer();    // TODO implement
    void shutDownGame();    // TODO implement

    void runCycle();
    void sendToMessageHandler();
    void receiveFromMessageHandler();

private:
    std::vector<int> m_activeGameList;
    std::vector<int> m_gameList;
    std::vector<std::string_view> m_incomingMessages; // message class (?)
    std::vector<std::string_view> m_outgoingMessages; // message class (?)
};

#endif