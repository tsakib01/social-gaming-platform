#include "MessageHandler.h"

#include <random>
#include <iostream>

std::deque<Message> 
MessageHandler::buildOutgoing(const std::deque<Message>& incoming) {
    std::deque<Message> outgoing;

	for (const Message& message : incoming) {
		User user = *(m_userManager->findUserByID(message.connection));
		outgoing.push_back(stateMap[user.state](message));
    }

    return outgoing;
}

Message 
MessageHandler::ProcessNew(const Message& message) {
	if (message.text == "J") {
		m_userManager->setUserState(message.connection, UserState::JOIN);
		return Message{message.connection, "Please enter a room code.\n"};
	}

	else if (message.text == "C") {
		m_userManager->setUserState(message.connection, UserState::CREATE);
		return Message{message.connection, "Choose a game to play.\n"}; 
	}

	else {
		return Message{message.connection, "Invalid, try again.\n"};
	}
}
    
Message
MessageHandler::ProcessJoin(const Message& message) {
	// message to send "Please enter a room code."
	// accepted inputs: room codes
	
	std::vector<User> users = m_userManager->getAllUsers();
	// bool gameFound = std::any_of(users.begin(), users.end(), [message.];

	if (true) {
		return Message{message.connection, "Joined game. Waiting on host...\n"};
	}

	else {
		return Message{message.connection, "Invalid, try again.\n"};
	}
}

Message
MessageHandler::ProcessCreate(const Message& message) {
	// message to send: "Choose a game to play."
	// accepted inputs: List of games

	if (message.text == "games/rock-paper-scissors.game") {
		// Instantiate game
		// GameInstanceManager.createGame("games/rock-paper-scissors.game");
		// Return room code to the user
	}

	else {
		return Message{message.connection, "Invalid, try again.\n"};
	}
}