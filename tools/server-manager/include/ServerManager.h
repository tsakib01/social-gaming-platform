
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

    std::deque<Message> buildOutgoing(const std::deque<Message>& incoming);
    
    Message ProcessNewState(const Message& message);
    Message ProcessJoinState(const Message& message);
    Message ProcessCreateState(const Message& message);

    std::map<UserState, std::function<Message(const Message&)>> stateMap = {
        {UserState::NEW, std::bind(&ServerManager::ProcessNewState, this, std::placeholders::_1)},
        {UserState::JOIN, std::bind(&ServerManager::ProcessJoinState, this, std::placeholders::_1)},
        {UserState::CREATE, std::bind(&ServerManager::ProcessCreateState, this, std::placeholders::_1)}
    };
};