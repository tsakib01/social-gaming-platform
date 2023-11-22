
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
#include <filesystem>
#include <sstream>

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

    // Takes in a list of Messages, and returns what the user said in a 
    // cleanly formatted way to display on screen
    std::deque<Message> buildUserMessages(const std::deque<Message>& incoming);

    // Takes in a list of Messages, and builds a list of Message responses
    // Based on the state of the User, it will call a corresponding ProcessX()
    // method defined in stateMap to handle it appropriately
    std::deque<Message> buildResponses(const std::deque<Message>& incoming);
    
    // Handles validity checking and returns a Message accordingly
    std::deque<Message> processNew(const Message& message);
    std::deque<Message> processIntro(const Message& message);
    std::deque<Message> processJoinGame(const Message& message);
    std::deque<Message> processGameSelect(const Message& message);
    std::deque<Message> processGameConfig(const Message& message);
    std::deque<Message> processGameWait(const Message& message);
    std::deque<Message> processGameRunning(const Message& message);
    
    std::deque<Message> buildGroupMessage(const std::vector<User>& gameUsers, const std::string& message);
    Message buildGameFiles(const Message& message);
    std::vector<std::string> getGameFiles();

    std::map<UserState, std::function<std::deque<Message>(const Message&)>> stateMap = {
        {UserState::NEW, std::bind(&ServerManager::processNew, this, std::placeholders::_1)},
        {UserState::INTRO, std::bind(&ServerManager::processIntro, this, std::placeholders::_1)},
        {UserState::JOIN_GAME, std::bind(&ServerManager::processJoinGame, this, std::placeholders::_1)},
        {UserState::GAME_SELECT, std::bind(&ServerManager::processGameSelect, this, std::placeholders::_1)},
        {UserState::GAME_CONFIG, std::bind(&ServerManager::processGameConfig, this, std::placeholders::_1)},
        {UserState::GAME_WAIT, std::bind(&ServerManager::processGameWait, this, std::placeholders::_1)},
        {UserState::GAME_RUN, std::bind(&ServerManager::processGameRunning, this, std::placeholders::_1)}
    };
};