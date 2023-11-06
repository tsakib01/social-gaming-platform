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

		std::deque userMessages = buildUserMessages(incoming);
		server->send(userMessages);
		
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
  server->send(std::deque<Message>(1, {client, "Welcome! What is your name?\n"}));
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
ServerManager::buildUserMessages(const std::deque<Message>& incoming) {
	std::deque<Message> messages;

	for (const Message& message : incoming) {
		messages.push_back(Message{message.connection, 
			"> " + message.text + "\n"});
	}

	return messages;
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
			"Type (J) to join, (C) to create a game.\n"};
	}

	else {
		return Message{message.connection, 
			"Invalid, try again.\n"};
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
		return ProcessGameFiles(message); 
	}

	else {
		return Message{message.connection, 
			"Invalid, try again.\n"};
	}
}
    
Message
ServerManager::ProcessJoinGame(const Message& message) {	
	try {
		uint16_t code = std::stoi(message.text);
		std::vector<User> users = userManager->getAllUsers();
		auto it = std::find_if(users.begin(), users.end(), [code](const User& user) {
			return user.roomCode == code;
        });

		if (it != users.end()) {
			userManager->setUserRole(message.connection, Role::PLAYER);
			userManager->setUserRoomCode(message.connection, code);
			userManager->setUserState(message.connection, UserState::GAME_WAIT);
			return Message{message.connection, 
				"Joined game. Waiting on host...\n"};
		} 
		
		else {
			return Message{message.connection, 
				"Room not found, try again.\n"};
		}
	} 

	catch (const std::invalid_argument& e) {
		return Message{message.connection, 
			"Please enter a valid number.\n"};
	}
}

Message
ServerManager::ProcessGameSelect(const Message& message) {
	std::vector<std::string> games = GetGameFiles();

	try {
		uint8_t choice = std::stoi(message.text);
		if (choice > 0 && choice <= games.size()) {
			uint16_t roomCode = gameInstanceManager->generateRoomCode();
			userManager->setUserRole(message.connection, Role::OWNER);
			userManager->setUserRoomCode(message.connection, roomCode);
			userManager->setUserState(message.connection, UserState::GAME_WAIT);
			return Message{message.connection, 
				"Room Code: " + std::to_string(roomCode) + ".     Type (S) to start.\n"};
		}

		else {
			return Message{message.connection, 
				"Invalid, try again.\n"};
		}
	}

	catch (const std::invalid_argument& e) {
		return Message{message.connection,
			"Please enter an option.\n"};
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
		return Message{message.connection, 
			"Moving to GAME_RUN state\n"};
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

Message 
ServerManager::ProcessGameFiles(const Message& message){
	std::vector<std::string> gameFiles = GetGameFiles();
	std::string gameFilesToPrint = "Enter the number for which game you'd like to play:\n";
	int fileCount = 1;
	std::stringstream stringStream;
	
	for (std::string file : gameFiles){
		gameFilesToPrint.append(std::to_string(fileCount) + ": " + file + "\n");
		fileCount++;
	}

	return Message{message.connection, gameFilesToPrint};
}

std::vector<std::string> 
ServerManager::GetGameFiles(){
	std::vector<std::string> gameFiles;
	std::string gamePath = "games";
	
	for (const auto & gameFile: std::filesystem::directory_iterator(gamePath)){
		gameFiles.push_back(gameFile.path());
	}

	return gameFiles;
}