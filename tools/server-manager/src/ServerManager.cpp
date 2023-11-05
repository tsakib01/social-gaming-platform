#include "ServerManager.h"

ServerManager::ServerManager(const unsigned short port, const char* htmlFile) {
    server = std::make_unique<Server>(
                  port, getHTTPMessage(htmlFile),
                  std::bind(&ServerManager::onConnect, this, std::placeholders::_1),
                  std::bind(&ServerManager::onDisconnect, this, std::placeholders::_1));
    gameInstanceManager = std::make_unique<GameInstanceManager>();
    userManager = std::make_shared<UserManager>();
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

        std::deque outgoing = buildOutgoing(incoming);
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
    return std::string{std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
  }

  std::cerr << "Unable to open HTML index file:\n" << htmlLocation << "\n";
  std::exit(-1);
}

std::deque<Message> 
ServerManager::buildOutgoing(const std::deque<Message>& incoming) {
    std::deque<Message> outgoing;

	for (const Message& message : incoming) {
		User user = *(userManager->findUserByID(message.connection));
		outgoing.push_back(stateMap[user.state](message));
    }

    return outgoing;
}

Message 
ServerManager::ProcessNewState(const Message& message) {
	if (message.text == "J") {
		userManager->setUserState(message.connection, UserState::JOIN);
		return Message{message.connection, "Please enter a room code.\n"};
	}

	else if (message.text == "C") {
		userManager->setUserState(message.connection, UserState::CREATE);
		return Message{message.connection, "Choose a game to play.\n"}; 
	}

	else {
		return Message{message.connection, "Invalid, try again.\n"};
	}
}
    
Message
ServerManager::ProcessJoinState(const Message& message) {	
    std::vector<User> users = userManager->getAllUsers();
    // bool gameFound = std::any_of(users.begin(), users.end(), [message.];

    if (true) {
      return Message{message.connection, "Joined game. Waiting on host...\n"};
    }

    else {
      return Message{message.connection, "Invalid, try again.\n"};
    }
}

Message
ServerManager::ProcessCreateState(const Message& message) {
	if (message.text == "games/rock-paper-scissors.game") {
		// gameInstanceManager.createGame("games/rock-paper-scissors.game");
		// return game room code to user
	}

	else {
		return Message{message.connection, "Invalid, try again.\n"};
	}
}