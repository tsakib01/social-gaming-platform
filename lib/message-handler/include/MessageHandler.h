#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "Server.h"

#include <vector>
#include <string>

enum class ClientAction : char {
    CREATE_ROOM, JOIN_GAME, LEAVE_ROOM, START_GAME, END_GAME, HANDLE_INPUT   
};

struct ClientToRoom {
    std::string clientID;
    std::string roomCode;
    std::string role;
    std::string gameID;
};

struct ClientN {
    networking::Connection client;
    std::string role;
    std::string gameID;
}

struct Reponse {
    std::string message;
}


class MessageHandler {
public:
    MessageHandler(const Server& server);
    std::any& receive(ClientAction& action, ClientToRoom& clientToRoom);

private:
    std::unordered_map<std::string, std::vector<ClientN>> clients;
    Server& server;

    std::string& generateRoomCode();
    std::string& createRoom(ClientToRoom& clientToRoom);
    Reponse& addClient(ClientToRoom& clientToRoom) const;
    Response& removeClient(ClientToRoom& ClientToRoom) const;
    Response& startGame(std::string roomCode, u_int8_t numOfPlayer);
    Reponse& endGame(std::string roomCode);
    Response& handleInputs(std::string roomCode);
};