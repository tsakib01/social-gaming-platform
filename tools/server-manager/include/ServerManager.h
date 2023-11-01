
#include "Server.h"
#include "UserManager.h"
#include "GameInstanceManager.h"

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
    std::shared_ptr<Server> server;
    std::unique_ptr<GameInstanceManager> gameInstanceManager;
    std::unique_ptr<UserManager> userManager;

    void onConnect(Connection client);
    void onDisconnect(Connection client);
    std::deque<Message> buildOutgoing(const std::string_view& log);
    std::string getHTTPMessage(const char* htmlLocation);
};