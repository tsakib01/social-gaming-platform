#include "ServerManager.h"

std::deque<Message>  
ServerManager::processNew(const Message& message) {
	if (!message.text.empty()) {
		userManager->setUserName(message.connection, message.text);
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
ServerManager::processIntro(const Message& message) {
	if (message.text == "J") {
		userManager->setUserState(message.connection, UserState::JOIN_GAME);
		return std::deque<Message>{
			{message.connection, "Please enter a room code.\n"}};
	}

	else if (message.text == "C") {
		userManager->setUserState(message.connection, UserState::GAME_SELECT);
		return std::deque<Message>{
			{buildGameFiles(message)}}; 
	}

	else {
		return std::deque<Message>{
			{message.connection, "Invalid, try again.\n"}};
	}
}
    
std::deque<Message>
ServerManager::processJoinGame(const Message& message) {	
	try {
		uint16_t code = std::stoi(message.text);
		std::vector<uint16_t> roomCodes = gameInstanceManager->getRoomCodes();

		if (std::find(roomCodes.begin(), roomCodes.end(), code) != roomCodes.end()) {
			userManager->setUserRole(message.connection, Role::PLAYER);
			userManager->setUserRoomCode(message.connection, code);
			userManager->setUserState(message.connection, UserState::GAME_WAIT);
	
			User player = userManager->getUserByID(message.connection);
			User host = userManager->getRoomOwner(code);

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
ServerManager::processGameSelect(const Message& message) {
	try {
		uint8_t choice = std::stoi(message.text);

		std::vector<std::string> games = getGameFiles();
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
ServerManager::processGameConfig(const Message& message) {
	// Not implemented yet.
}

std::deque<Message>
ServerManager::processGameWait(const Message& message) {
	User user = userManager->getUserByID(message.connection);

	if (message.text == "S" && user.role == Role::OWNER) {
		userManager->setUserState(message.connection, UserState::GAME_RUN);
		gameInstanceManager->startGame(user.roomCode, userManager->getUsersInGame(user.roomCode));
		return buildGroupMessage(userManager->getUsersInGame(user.roomCode), "Starting game...\n");
	}

	else {
		return std::deque<Message>{
			{message.connection, ""}};
	}
}

std::deque<Message>
ServerManager::processGameRunning(const Message& message) {
	// Not implemented yet.

	return std::deque<Message>{
		{message.connection, "Inside GAME_RUN state.\n"}};
}

std::deque<Message>
ServerManager::buildGroupMessage(const std::vector<User>& gameUsers, const std::string& message) {
	std::deque<Message> messages;
	for (const User& user : gameUsers) {
		messages.push_back(
			Message{user.userID, message});
	}
	return messages;
}


Message 
ServerManager::buildGameFiles(const Message& message){
	std::vector<std::string> gameFiles = getGameFiles();
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
ServerManager::getGameFiles(){
	std::vector<std::string> gameFiles;
	std::string gamePath = "games";
	
	for (const auto & gameFile: std::filesystem::directory_iterator(gamePath)){
		gameFiles.push_back(gameFile.path());
	}

	return gameFiles;
}