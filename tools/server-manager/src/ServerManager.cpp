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
	server->send(std::deque<Message> {
		{client, "Welcome! What is your name?\n"}});
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
		std::deque<Message> append = stateMap[user.state](message);
		responses.insert(responses.end(), append.begin(), append.end());
    }

    return responses;
}

std::deque<Message>  
ServerManager::ProcessNew(const Message& message) {
	if (!message.text.empty()) {
		userManager->setUserName(message.connection, message.text);

		std::cout << "ProcessNew: " << message.text << std::endl;
		
		userManager->setUserState(message.connection, UserState::INTRO);
		return std::deque<Message>{
			{message.connection, "Type (J) to join, (C) to create a game.\n"}};
	}

	else {
		return std::deque<Message>{
			{message.connection, "Invalid, try again.\n"}};
	}
}

std::deque<Message> 
ServerManager::ProcessIntro(const Message& message) {
	if (message.text == "J") {
		userManager->setUserState(message.connection, UserState::JOIN_GAME);
		return std::deque<Message>{
			{message.connection, "Please enter a room code.\n"}};
	}

	else if (message.text == "C") {
		userManager->setUserState(message.connection, UserState::GAME_SELECT);
		return std::deque<Message>{
			{BuildGameFiles(message)}}; 
	}

	else {
		return std::deque<Message>{
			{message.connection, "Invalid, try again.\n"}};
	}
}
    
std::deque<Message>
ServerManager::ProcessJoinGame(const Message& message) {	
	try {
		uint16_t code = std::stoi(message.text);
		std::vector<uint16_t> roomCodes = gameInstanceManager->getRoomCodes();

		if (std::find(roomCodes.begin(), roomCodes.end(), code) != roomCodes.end()) {
			userManager->setUserRole(message.connection, Role::PLAYER);
			userManager->setUserRoomCode(message.connection, code);
			userManager->setUserState(message.connection, UserState::GAME_WAIT);
	
			User player = *(userManager->findUserByID(message.connection));
			User host = *(userManager->getRoomOwner(code));

			return std::deque<Message>{ 
				{message.connection, "Joined game. Waiting on host...\n"},
				{host.userID, "[" + std::string(player.username) + "] joined.\n"}};
		} 
		
		else {
			return std::deque<Message>{
				{message.connection, "Room not found, try again.\n"}};
		}
	} 

	catch (const std::invalid_argument& e) {
		return std::deque<Message>{
			{message.connection, "Please enter a valid number.\n"}};
	}
}

std::deque<Message>
ServerManager::ProcessGameSelect(const Message& message) {
	try {
		uint8_t choice = std::stoi(message.text);

		std::vector<std::string> games = GetGameFiles();
		if (choice > 0 && choice <= games.size()) {
			uint16_t roomCode = gameInstanceManager->createGameInstance(games[choice-1]);
			userManager->setUserRole(message.connection, Role::OWNER);
			userManager->setUserRoomCode(message.connection, roomCode);
			userManager->setUserState(message.connection, UserState::GAME_WAIT);
			return std::deque<Message>{
				{message.connection, "Room Code: " + std::to_string(roomCode) + "     Type (S) to start.\n"}};
		}

		else {
			return std::deque<Message>{
				{message.connection, "Invalid, try again.\n"}};
		}
	}

	catch (const std::invalid_argument& e) {
		return std::deque<Message>{
			{message.connection, "Please enter an option.\n"}};
	}
}

std::deque<Message>
ServerManager::ProcessGameConfig(const Message& message) {
	// Not implemented yet.
}

std::deque<Message>
ServerManager::ProcessGameWait(const Message& message) {
	User user = *(userManager->findUserByID(message.connection));

	if (message.text == "S" && user.role == Role::OWNER) {
		userManager->setUserState(message.connection, UserState::GAME_RUN);
		// gameInstanceManager->startGame(roomCode);
		return std::deque<Message>{
			{message.connection, "Moving to GAME_RUN state\n"}};
	}

	else {
		return std::deque<Message>{
			{message.connection, ""}};
	}
}

std::deque<Message>
ServerManager::ProcessGameRunning(const Message& message) {
	// Not implemented yet.

	return std::deque<Message>{
		{message.connection, "Inside GAME_RUN state.\n"}};
}

Message 
ServerManager::BuildGameFiles(const Message& message){
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