
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

/**
* ServerManager is the main component of the program that is in charge of running instructions, 
* receiving and sending messages, and keeping track of users in the system.
*/
class ServerManager {
public:
    ServerManager(const unsigned short port, const char* htmlFile);
    void startServer();

private:
    std::unique_ptr<Server> server;
    std::unique_ptr<GameInstanceManager> gameInstanceManager;
    std::shared_ptr<UserManager> userManager;
    std::unique_ptr<GameCommunicator> gameCommunicator;

    // When a new client connects from server, add them to user list
    void onConnect(Connection client);

    // When a client disconnects from server, remove them from user list
    void onDisconnect(Connection client);
    
    // Used to access webchat.html
    std::string getHTTPMessage(const char* htmlLocation);

    // Takes in a list of Messages, and builds a list of Message responses
    // Based on the state of the User, it will call a corresponding ProcessX() method
    // defined in stateMap to handle it appropriately
    std::deque<Message> buildResponses(const std::deque<Message>& incoming);
    
    // Handles validity checking and returns a Message accordingly
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