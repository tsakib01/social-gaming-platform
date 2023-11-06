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

struct Actions {
  uint8_t roomCode;
  std::map<std::string_view, std::vector<std::string_view>> actions; // {"choices", {"rock","paper","scissors"}}
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
    GameCommunicator(std::shared_ptr<UserManager>& userManager) : userManager{userManager} {}
    
    void assignAction(uint8_t roomCode, const std::map<std::string_view, std::vector<std::string_view>>& actions); //called by game instance
    std::vector<std::string_view> getOptionsForUser(const Message& message, std::string_view key) const;
    std::string_view getCurrentUserInput(Connection userID) const;
    void storeCurrentUserInput(const Message& message); // stores the current user input to use later for sending all at once to the game instance

private:
    std::shared_ptr<UserManager> userManager;
    std::vector<Actions> actions;
    std::vector<UserInput> userInputs; 
};

#endif