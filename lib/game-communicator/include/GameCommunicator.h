#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "Server.h"
#include "UserManager.h"

#include <vector>
#include <string>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

using networking::Message;
using networking::Server;

// struct Actions {
  // uint8_t roomCode;
  // std::map<std::string_view, std::vector<std::string_view>> actions;
  // < "choices" /// "rock","paper","scissors" >
// }

/**
 * GameCommunicator is in charge of receiving Messages and Inputs from GameInstances.
 * It will store these values into lists, and then be checked from the ServerManager
 * for error handling and receiving inputs from players.
*/
class GameCommunicator {
public:
    GameCommunicator() {}
    
    // void assignAction(uint8_t roomCode, std::vector<std::string_view>> actions); //called by game instance

private:
    // std::shared_ptr<UserManager> m_userManager;
    // std::vector<Actions> actions;
    // std::map<Connection, std::string_view> userToInputMap; 
};

#endif