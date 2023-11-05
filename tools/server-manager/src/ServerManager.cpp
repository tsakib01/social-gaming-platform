#include "ServerManager.h"

ServerManager::ServerManager(const unsigned short port, const char* htmlFile) {
    server = std::make_unique<Server>(
                  port, getHTTPMessage(htmlFile),
                  std::bind(&ServerManager::onConnect, this, std::placeholders::_1),
                  std::bind(&ServerManager::onDisconnect, this, std::placeholders::_1));
    gameInstanceManager = std::make_unique<GameInstanceManager>();
    userManager = std::make_shared<UserManager>();
    messageHandler = std::make_unique<MessageHandler>(userManager);
}

void 
ServerManager::startServer() {
    while (true) {
        bool errorWhileUpdating = false;
        try {
            server->update();
        } catch (std::exception& e) {
            std::cerr << "Exception from Server update:\n" << " " << e.what() << "\n\n";
            errorWhileUpdating = true;
        }

        const auto incoming = server->receive();
        

        // gameInstanceManager->runCycle();

        std::deque outgoing = messageHandler->buildOutgoing(incoming);
        server->send(outgoing);

        sleep(1);
    }
}

void 
ServerManager::onConnect(Connection client) {
  std::cout << "New connection: " << client.id << "\n";
  userManager->addUser(client);
  server->send(std::deque<Message>(1, {client, "Welcome! Type J to join, C to create a game.\n"}));
}

void 
ServerManager::onDisconnect(Connection client) {
  std::cout << "Connection lost: " << client.id << "\n";
  userManager->removeUser(client);
}

std::string 
ServerManager::getHTTPMessage(const char* htmlLocation) {
  if (access(htmlLocation, R_OK ) != -1) {
    std::ifstream infile{htmlLocation};
    return std::string{std::istreambuf_iterator<char>(infile),
                       std::istreambuf_iterator<char>()};

  }

  std::cerr << "Unable to open HTML index file:\n"
            << htmlLocation << "\n";
  std::exit(-1);
}