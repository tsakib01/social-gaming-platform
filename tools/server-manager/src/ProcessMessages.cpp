#include "ServerManager.h"

std::deque<Message>  
ServerManager::processNew(const Message& message) {
	if (!message.text.empty()) {
		userManager->setUserName(message.connection, message.text);
		userManager->setUserState(message.connection, UserState::INTRO);
		return std::deque<Message>{
			{message.connection, "(J) to join, (C) to create a game.\n", true}};
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
			{message.connection, "Please enter a room code, or type (B) to cancel.\n"}};
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

		if (std::find(roomCodes.begin(), roomCodes.end(), code) != roomCodes.end() && gameInstanceManager->gameIsJoinable(code)) {
			userManager->setUserRole(message.connection, Role::PLAYER);
			userManager->setUserRoomCode(message.connection, code);
			userManager->setUserState(message.connection, UserState::GAME_WAIT);
	
			User player = userManager->getUserByID(message.connection);
			User owner = userManager->getRoomOwner(code);

			return std::deque<Message>{ 
				{message.connection, "Joined game. Waiting on owner...     Type (B) to leave.\n"},
				{owner.userID, "[" + std::string(player.username) + "] joined.\n"}};
		} 
		
		else {
			return std::deque<Message>{
				{message.connection, "Invalid, try again.\n"}};
		}
	} 

	catch (const std::invalid_argument& e) {		
		if(message.text == "B"){
			return processNew(message);
		}
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
			uint16_t roomCode = gameInstanceManager->createGameInstance(games[choice-1], gameCommunicator);
			userManager->setUserRole(message.connection, Role::OWNER);
			userManager->setUserRoomCode(message.connection, roomCode);

			if (gameInstanceManager->gameHasSetup(roomCode)) {
				userManager->setUserState(message.connection, UserState::GAME_CONFIG);
				const auto [prompt, valid, finished] = gameInstanceManager->inputConfig(roomCode, "");
				return std::deque<Message>{
					{message.connection, prompt + "\n"}};
			} 
			else {
				userManager->setUserState(message.connection, UserState::GAME_WAIT);
				return std::deque<Message>{
					{message.connection, "Room Code: " + std::to_string(roomCode) + "     Type (S) to start, (B) to quit.\n"}};
			}
		}

		else {
			return std::deque<Message>{
				{message.connection, "Invalid, try again.\n"}};
		}
	}

	catch (const std::invalid_argument& e) {
		if(message.text == "B"){
			return processNew(message);
		}
		return std::deque<Message>{
			{message.connection, "Please enter an option.\n"}};
	}
}

std::deque<Message>
ServerManager::processGameConfig(const Message& message) {	
	User owner = userManager->getUserByID(message.connection);
	const auto [prompt, validResponse, finished] = gameInstanceManager->inputConfig(owner.roomCode, message.text);

	if (finished.status) {
		userManager->setUserState(message.connection, UserState::GAME_WAIT);
		return std::deque<Message>{
			{message.connection, "Room Code: " + std::to_string(owner.roomCode) + "     Type (S) to start, (B) to quit.\n"}};
	}

	else if (!finished.status && validResponse.status) {
		return std::deque<Message>{
			{message.connection, prompt + "\n"}};
	}

	else {
		return std::deque<Message>{
			{message.connection, "Invalid, try again.\n"}};
	}
}

std::deque<Message>
ServerManager::processGameWait(const Message& message) {
	User user = userManager->getUserByID(message.connection);

	if (message.text == "S" && user.role == Role::OWNER) {
		userManager->setUserState(message.connection, UserState::GAME_RUN);
		gameInstanceManager->startGame(user.roomCode, userManager->getUsersInGame(user.roomCode));
		return buildGroupMessage(userManager->getUsersInGame(user.roomCode), "Starting game...\n");
	}

	if (message.text == "B" && user.role == Role::PLAYER) {
		User owner = userManager->getRoomOwner(user.roomCode);
		userManager->setUserRole(message.connection, Role::NONE);
		userManager->setUserRoomCode(message.connection, -1);
		userManager->setUserState(message.connection, UserState::JOIN_GAME);
		return std::deque<Message>{
			{message.connection, "You have left the game.\nPlease enter a room code, or type (B) to cancel.\n"},
			{owner.userID, "[" + std::string(user.username) + "] left.\n"}};
	}

	if (message.text == "B" && user.role == Role::OWNER) {
		gameInstanceManager->deleteGame(user.roomCode);

		std::vector<User> users = userManager->getUsersInGame(user.roomCode);
		for (User gameUser : users) {
			userManager->setUserRole(gameUser.userID, Role::NONE);
			userManager->setUserRoomCode(gameUser.userID, -1);
			userManager->setUserState(gameUser.userID,
				(gameUser.role == Role::PLAYER) ? UserState::JOIN_GAME : UserState::GAME_SELECT);
		}

		users.erase(std::remove_if(users.begin(), users.end(), [](const User& user) {
    		return user.role == Role::OWNER;
		}), users.end());
		
		std::deque<Message> playerMsgs = buildGroupMessage(users, "Game has been cancelled.\nPlease enter a room code, or type (B) to cancel.\n");
		Message ownerMsg = {message.connection, "Game has been cancelled.\n"};
		Message gameFiles = buildGameFiles(message);

		std::deque<Message> result;
		result.insert(result.end(), playerMsgs.begin(), playerMsgs.end());
		result.push_back(ownerMsg);
		result.push_back(gameFiles);
		return result;
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
	std::string gameFilesToPrint = "Enter the number for which game you'd like to play, or type (B) to cancel:\n";
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