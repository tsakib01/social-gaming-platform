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

struct MessageResult {
  std::string result;
  bool shouldShutdown;
};

// struct Actions {
  // uint8_t roomCode;
  // std::map<std::string_view, std::vector<std::string_view>> actions;
  // < "choices" /// "rock","paper","scissors" >
// }

class MessageHandler {
public:
    MessageHandler(std::shared_ptr<UserManager> userManager) : m_userManager(userManager) {}
    
    std::deque<Message> handleMessage(const std::deque<Message>& incoming);

    std::deque<Message> buildOutgoing(const std::string_view& log);
    // MessageResult processMessages(std::unique_ptr<Server> server, const std::deque<Message>& incoming);


    // void assignAction(uint8_t roomCode, std::vector<std::string_view>> actions); //called by game instance

private:
    std::shared_ptr<UserManager> m_userManager;
    // std::vector<Actions> actions;
    // std::map<Connection, std::string_view> userToInputMap; 

    Message ProcessNew(const Message& message);
    Message ProcessJoin(const Message& message);
    Message ProcessCreate(const Message& message);
    // Message ProcessInput(const Message& message);

    std::map<UserState, std::function<Message(const Message&)>> stateMap = {
      {UserState::NEW, std::bind(&MessageHandler::ProcessNew, this, std::placeholders::_1)},
      {UserState::JOIN, std::bind(&MessageHandler::ProcessJoin, this, std::placeholders::_1)},
      {UserState::CREATE, std::bind(&MessageHandler::ProcessCreate, this, std::placeholders::_1)}
      // {UserState::GAME_INPUT, std::bind(&MessageHandler::ProcessInput, this, std::placeholders::_1)}
    };
};

#endif