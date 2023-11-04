#include "MessageHandler.h"

#include <random>
#include <iostream>
MessageHandler::MessageHandler() : server{server} {};

std::string
MessageHandler::generateRoomCode() {
    std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string roomCode;
    for(int i = 0; i < 8; ++i) {
        roomCode += characters[distribution(generator)];
    }

    return roomCode;
}


std::string& 
MessageHandler::createRoom(ClientToRoom::ClientToRoom& clientToRoom) {
    auto roomCode = generateRoomCode(); 

    ClientN::ClientN client;
    client.client.id = clientToRoom.clientID;
    client1.role = clientToRoom.role;

    clients[roomCode].push_back(client);
    std::cout << MessageHandler::messages.find(ROOM_CREATED_MESSAGE)->second << roomCode << std::endl;
    return roomCode;
}

Response::Reponse& 
MessageHandler::addClient(ClientToRoom::ClientToRoom& clientToRoom) const {
    return "Not Implemented Yet";
}

Response::Response&
MessageHandler::removeClient(ClientToRoom::ClientToRoom& ClientToRoom) const {
    return "Not Implemented Yet";
}

Response::Response&
MessageHandler::startGame(std::string roomID, u_int8_t numOfPlayer) {
    return "Not Implemented Yet";
}

Response::Reponse& 
MessageHandler::endGame(std::string roomID) {
    return "Not Implemented Yet";
}

Response::Response& 
MessageHandler::handleInputs(std::string roomID) {
    return "Not Implemented Yet";
}

std::any
MessageHandler::receive(ClientAction::ClientAction& action, ClientToRoom::ClientToRoom& clientToRoom) {
    switch(action) {
        case action::CREATE_ROOM:   return createRoom();
        case action::JOIN_GAME:     return addClient(clientToRoom);
        case action::LEAVE_ROOM:    return removeClient(clientToRoom);
        case action::START_GAME:    return startGame(std::string roomCode, u_int8_t numOfPlayer);
        case action::END_GAME:      return endGame(std::string roomCode);
        case action::HANDLE_INPUT:  return handleInput(std::string roomCode);
    }
}