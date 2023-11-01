#include "ServerManager.h"

ServerManager::ServerManager(const unsigned short port, const char* htmlFile) {
    server = std::make_shared<Server>(
                  port, getHTTPMessage(htmlFile),
                  std::bind(&ServerManager::onConnect, this, std::placeholders::_1),
                  std::bind(&ServerManager::onDisconnect, this, std::placeholders::_1));
    gameInstanceManager = std::make_unique<GameInstanceManager>();
    userManager = std::make_unique<UserManager>();
}

struct MessageResult {
  std::string result;
  bool shouldShutdown;
};

MessageResult
processMessages(std::shared_ptr<Server> server, const std::deque<Message>& incoming) {
  std::ostringstream result;
  bool quit = false;
  for (const auto& message : incoming) {
    if (message.text == "quit") {
      server->disconnect(message.connection);
    } else if (message.text == "shutdown") {
      std::cout << "Shutting down.\n";
      quit = true;
    } else {
      result << message.connection.id << "> " << message.text << "\n";
    }
  }
  return MessageResult{result.str(), quit};
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
        const auto [log, shouldQuit] = processMessages(server, incoming);

        const auto outgoing = buildOutgoing(log);
        server->send(outgoing);

        if (shouldQuit || errorWhileUpdating) {
            break;
        }

        sleep(1);
    }
}

void 
ServerManager::onConnect(Connection client) {
  std::cout << "New connection: " << client.id << "\n";
  userManager->addUser(client);
}

void 
ServerManager::onDisconnect(Connection client) {
  std::cout << "Connection lost: " << client.id << "\n";
  userManager->removeUser(client);
}


std::deque<Message>
ServerManager::buildOutgoing(const std::string_view& log) {
  std::deque<Message> outgoing;
  std::vector<User> clients = userManager->getAllUsers();
  for (auto& client : clients) {
      Message message;
      message.connection = client.userID;
      message.text = log;
      outgoing.push_back(message);
  }
  return outgoing;
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