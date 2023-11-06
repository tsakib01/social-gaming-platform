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
ServerManager::buildOutgoing(const std::deque<Message>& incoming) {
    std::deque<Message> outgoing;

	for (const Message& message : incoming) {
		User user = *(userManager->findUserByID(message.connection));
		outgoing.push_back(stateMap[user.state](message));
    }

    return outgoing;
}

Message 
ServerManager::ProcessNew(const Message& message) {
	if (message.text != "") {
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
		return Message{message.connection, "Please enter a room code.\n"};
	}

	else if (message.text == "C") {
		userManager->setUserState(message.connection, UserState::GAME_CREATE);
		return Message{message.connection, "Choose a game to play.\n"}; 
	}

	else {
		return Message{message.connection, "Invalid, try again.\n"};
	}
}
    
Message
ServerManager::ProcessJoinGame(const Message& message) {	
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
ServerManager::ProcessGameCreate(const Message& message) {
	if (message.text == "games/rock-paper-scissors.game") {
		gameInstanceManager->createGameInstance(message.text);
		userManager->setUserState(message.connection, UserState::GAME_WAIT);
		return Message{message.connection, 
			"Here is your room code: UNFINISHED! Type \"start\" when ready.\n"};
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
	// Not implemented yet.
	if (message.text == "start") {
		userManager->setUserState(message.connection, UserState::GAME_RUN);
		return Message{message.connection, "Moving to GAME_RUN state\n"};
	}

	else {
		return Message{message.connection, "Invalid, try again.\n"};
	}
}

Message
ServerManager::ProcessGameRunning(const Message& message) {
	// Not implemented yet.
	return Message{message.connection, "Inside GAME_RUN state.\n"};
}

Message ServerManager::ProcessGameFiles(const Message& message){
	std::vector<std::string> gameFiles = GetGameFiles();
	std::string gameFilesToPrint = "Files found in \"games\" folder:\n";
	int fileCount = 1;
	std::stringstream stringStream;
	for (std::string file : gameFiles){
		gameFilesToPrint.append(std::to_string(fileCount) + ": " + file + "\n");
		fileCount++;
	}
	return Message{message.connection, gameFilesToPrint};
}

std::vector<std::string> ServerManager::GetGameFiles(){
	std::vector<std::string> gameFiles;
	std::string gamePath = "games";
	for(const auto & gameFile: std::filesystem::directory_iterator(gamePath)){
		gameFiles.push_back(gameFile.path());
	}
	return gameFiles;
}