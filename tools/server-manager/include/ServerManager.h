
#include "Server.h"
#include "UserManager.h"
#include "GameInstanceManager.h"
#include "GameCommunicator.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

using networking::Server;
using networking::Connection;
using networking::Message;


class ServerManager {
public:
    ServerManager(const unsigned short port, const char* htmlFile);
    void startServer();

private:
    std::unique_ptr<Server> server;
    std::unique_ptr<GameInstanceManager> gameInstanceManager;
    std::shared_ptr<UserManager> userManager;
    std::unique_ptr<GameCommunicator> gameCommunicator;

    void onConnect(Connection client);
    void onDisconnect(Connection client);
    std::string getHTTPMessage(const char* htmlLocation);

    // Takes in a list of Messages, and builds responses by going through them one 
    // at a time and calls the corresponding method in stateMap
    std::deque<Message> buildOutgoing(const std::deque<Message>& incoming);
    
    Message ProcessNew(const Message& message);
    Message ProcessIntro(const Message& message);
    Message ProcessJoinGame(const Message& message);
    Message ProcessGameSelect(const Message& message);
    Message ProcessGameConfig(const Message& message);
    Message ProcessGameWait(const Message& message);
    Message ProcessGameRunning(const Message& message);
    
    std::map<UserState, std::function<Message(const Message&)>> stateMap = {
        {UserState::NEW, std::bind(&ServerManager::ProcessNew, this, std::placeholders::_1)},
        {UserState::INTRO, std::bind(&ServerManager::ProcessIntro, this, std::placeholders::_1)},
        {UserState::JOIN_GAME, std::bind(&ServerManager::ProcessJoinGame, this, std::placeholders::_1)},
        {UserState::GAME_SELECT, std::bind(&ServerManager::ProcessGameSelect, this, std::placeholders::_1)},
        {UserState::GAME_CONFIG, std::bind(&ServerManager::ProcessGameConfig, this, std::placeholders::_1)},
        {UserState::GAME_WAIT, std::bind(&ServerManager::ProcessGameWait, this, std::placeholders::_1)},
        {UserState::GAME_RUN, std::bind(&ServerManager::ProcessGameRunning, this, std::placeholders::_1)}
    };
};