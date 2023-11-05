
#include "Server.h"
#include "UserManager.h"
#include "GameInstanceManager.h"
#include "MessageHandler.h"

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
    std::unique_ptr<MessageHandler> messageHandler;

    void onConnect(Connection client);
    void onDisconnect(Connection client);
    std::string getHTTPMessage(const char* htmlLocation);
};