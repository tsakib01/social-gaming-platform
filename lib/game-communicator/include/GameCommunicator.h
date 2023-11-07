#ifndef GAME_COMMUNICATOR_H
#define GAME_COMMUNICATOR_H

#include "Server.h"
#include "UserManager.h"

#include <vector>
#include <string>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <unordered_map>

using networking::Message;
using networking::Server;

struct GameChoices {
  uint8_t roomCode;
  std::vector<std::string_view> choices; // {"choices", {"rock","paper","scissors"}}
};

struct UserInput{
  Connection userID;
  std::string_view input;
};

/**
 * GameCommunicator is in charge of receiving Messages and Inputs from GameInstances.
 * It will store these values into lists, and then be checked from the ServerManager
 * for error handling and receiving inputs from players.
*/
class GameCommunicator {
public:
    GameCommunicator() {}
    
    void setGameChoices(uint16_t roomCode, const std::vector<std::string_view>& choices); //called by game instance
    void storeCurrentUserInput(const Message& message); // stores the current user input to use later for sending all at once to the game instance
    void setGameMessage(const std::vector<Connection> users, std::string_view message);

    std::vector<std::string_view> getChoicesForUser(uint16_t roomCode) const;
    std::string_view getCurrentUserInput(Connection userID) const;
    std::vector<Message> getGameMessages() const { return gameMessages; }

private:
    std::vector<GameChoices> gameChoices;
    std::vector<UserInput> userInputs; 
    std::vector<Message> gameMessages;
};

#endif