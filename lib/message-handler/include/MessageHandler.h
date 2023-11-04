#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "Server.h"

#include <vector>
#include <string>
#include <map>

// An identifier for all possible, preset messages that the MessageHandler can send.

enum class HandlerMessage {
    ENTRANCE_PROMPT, INVALID_COMMAND_ERROR, CREATE_PROMPT, CREATE_ERROR, JOIN_PROMPT, JOIN_ERROR, LEAVE_GAME_PROMPT,
    ROOM_CREATED_MESSAGE
};

    std::map<HandlerMessage, std::string> messages;
    messages[ENTRANCE_PROMPT] = "Welcome! Type J to join, C to create a game";
    messages[INVALID_COMMAND_ERROR] = "ERROR: Invalid command. Please try again."; 
    messages[CREATE_PROMPT] = "Please provide the required game file to set up your game.";
    messages[CREATE_ERROR] = "ERROR: The file you selected cannot be used. Please try again."
    messages[JOIN_PROMPT] = "ERROR: This room does not exist, please try again.";
    messages[JOIN_ERROR] = "ERROR: The room ID you entered could not be found. Please try again.";
    messages[LEAVE_GAME_PROMPT] = "Are you sure you would like to leave this game? Type Y for yes, N for no."
    messages[ROOM_CREATED_MESSAGE] = "Your room has been successfully created. Code for joining the game: ";

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