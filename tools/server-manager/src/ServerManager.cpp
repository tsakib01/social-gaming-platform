#include "ServerManager.h"

ServerManager::ServerManager(const unsigned short port, const char* htmlFile) {
    server = std::make_unique<Server>(
                  port, getHTTPMessage(htmlFile),
                  std::bind(&ServerManager::onConnect, this, std::placeholders::_1),
                  std::bind(&ServerManager::onDisconnect, this, std::placeholders::_1));
    gameInstanceManager = std::make_unique<GameInstanceManager>();
    userManager = std::make_shared<UserManager>();
	gameCommunicator = std::make_unique<GameCommunicator>();
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
		User user = *(userManager->getUserByID(message.connection));
		std::deque<Message> append = stateMap[user.state](message);
		responses.insert(responses.end(), append.begin(), append.end());
    }

    return responses;
}