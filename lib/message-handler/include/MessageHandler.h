#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "Server.h"

#include <vector>
#include <string>

enum class ClientAction : char {
    CREATE_ROOM, JOIN_GAME, LEAVE_ROOM, START_GAME, END_GAME, HANDLE_INPUT   
};

class MessageHandler {
public:
    MessageHandler();

private:
    Response& handleInputs(std::string roomCode);
};