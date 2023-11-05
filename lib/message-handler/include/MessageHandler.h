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
 * MessageHandler is in charge of receiving and creating Messages.
 * Based on the unique connection ID of each User, it will check which state of the
 * program the User is in, and call a corresponding method assigned to each state
 * that handles validity checking differently.
*/
class MessageHandler {
public:
    MessageHandler(std::shared_ptr<UserManager> userManager) {}

    // Takes in a list of Messages, and builds responses by going through them one 
    // at a time and calls the corresponding method in stateMap
    
    // void assignAction(uint8_t roomCode, std::vector<std::string_view>> actions); //called by game instance

private:
    // std::shared_ptr<UserManager> m_userManager;
    // std::vector<Actions> actions;
    // std::map<Connection, std::string_view> userToInputMap; 

    // Message ProcessInput(const Message& message);

    // std::map<UserState, std::function<Message(const Message&)>> stateMap = {
    //   {UserState::NEW, std::bind(&MessageHandler::ProcessNew, this, std::placeholders::_1)},
    //   {UserState::JOIN, std::bind(&MessageHandler::ProcessJoin, this, std::placeholders::_1)},
    //   {UserState::CREATE, std::bind(&MessageHandler::ProcessCreate, this, std::placeholders::_1)}
    //   // {UserState::GAME_INPUT, std::bind(&MessageHandler::ProcessInput, this, std::placeholders::_1)}
    // };
};

#endif