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

		gameInstanceManager->runCycle();

		const auto incoming = server->receive();
		
		// std::deque gameMessages = gameCommunicator->getMessages();
		// server->send(gameMessages);

        std::deque responses = buildResponses(incoming);
        server->send(responses);

        sleep(1);
    }
}

void 
ServerManager::onConnect(Connection client) {
  std::cout << "New connection: " << client.id << "\n";
  userManager->addUser(client);
  server->send(std::deque<Message>(1, {client, "Enter your name.\n"}));
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
ServerManager::buildResponses(const std::deque<Message>& incoming) {
    std::deque<Message> responses;

	for (const Message& message : incoming) {
		User user = *(userManager->findUserByID(message.connection));
		responses.push_back(stateMap[user.state](message));
    }

    return responses;
}

Message 
ServerManager::ProcessNew(const Message& message) {
	if (!message.text.empty()) {
		userManager->setUserName(message.connection, message.text);
		userManager->setUserState(message.connection, UserState::INTRO);
		return Message{message.connection, 
			"Welcome, " + message.text + "! Type J to join, C to create a game.\n"};
	}

	else {
		return Message{message.connection, "Invalid, try again.\n"};
	}
}

Message
ServerManager::ProcessIntro(const Message& message) {
	if (message.text == "J") {
		userManager->setUserState(message.connection, UserState::JOIN_GAME);
		return Message{message.connection, 
			"Please enter a room code.\n"};
	}

	else if (message.text == "C") {
		userManager->setUserState(message.connection, UserState::GAME_SELECT);
		return Message{message.connection, 
			"Choose a game to play.\n"}; 
	}

	else {
		return Message{message.connection, "Invalid, try again.\n"};
	}
}
    
Message
ServerManager::ProcessJoinGame(const Message& message) {	
	try {
		uint16_t number = std::stoi(message.text);
		std::vector<User> users = userManager->getAllUsers();
		auto it = std::find_if(users.begin(), users.end(), [number](const User& user) {
			return user.roomCode == number;
        });

		if (it != users.end()) {
			userManager->setUserRole(message.connection, Role::PLAYER);
			userManager->setUserRoomCode(message.connection, std::stoi(message.text));
			userManager->setUserState(message.connection, UserState::GAME_WAIT);
			return Message{message.connection, "Joined game. Waiting on host...\n"};
		}
	} 

	catch (const std::invalid_argument& e) {
		return Message{message.connection, "Please enter a number.\n"};
	}

	return Message{message.connection, "Room not found, try again.\n"};
}

Message
ServerManager::ProcessGameSelect(const Message& message) {
	// TODO: Instead of hard coding the path, make it a list of options grabbed from games directory
	if (message.text == "games/rock-paper-scissors.game") {
		uint16_t roomCode = gameInstanceManager->generateRoomCode();
		userManager->setUserRole(message.connection, Role::OWNER);
		userManager->setUserRoomCode(message.connection, roomCode);
		userManager->setUserState(message.connection, UserState::GAME_WAIT);
		return Message{message.connection, 
			"Room Code: " + std::to_string(roomCode) + "\tType S to start.\n"};
	}

	else {
		return Message{message.connection, "Invalid, try again.\n"};
	}
}

Message
ServerManager::ProcessGameConfig(const Message& message) {
	// Not implemented yet.
}

Message
ServerManager::ProcessGameWait(const Message& message) {
	User user = *(userManager->findUserByID(message.connection));

	if (message.text == "S" && user.role == Role::OWNER) {
		userManager->setUserState(message.connection, UserState::GAME_RUN);
		// gameInstanceManager->createGameInstance();
		return Message{message.connection, "Moving to GAME_RUN state\n"};
	}

	else {
		return Message{message.connection, ""};
	}
}

Message
ServerManager::ProcessGameRunning(const Message& message) {
	// Not implemented yet.
	return Message{message.connection, "Inside GAME_RUN state.\n"};
}