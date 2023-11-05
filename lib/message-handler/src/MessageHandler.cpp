#include "MessageHandler.h"

#include <random>
#include <iostream>


// struct Message {
//   Connection connection;
//   std::string text; //C, J 
// };


std::deque<Message> 
MessageHandler::handleMessage(const std::deque<Message>& incoming) {
    std::deque<Message> outgoing;
    
    for (const Message& message : incoming) {
      User user = *(m_userManager->findUserByID(message.connection));
      outgoing.push_back(stateMap[user.state](message));
    }

    return outgoing;
}

Message 
MessageHandler::ProcessNew(const Message& message) {
  if (message.text == "J") {
    m_userManager->setUserState(message.connection, UserState::JOIN);
    return Message{message.connection, "Please enter a room code.\n"};
  }
  else if (message.text == "C") {
    m_userManager->setUserState(message.connection, UserState::CREATE);
    return Message{message.connection, "Choose a game to play.\n"}; 
  }
  else {
    return Message{message.connection, "Invalid input.\n"};
  }
}
    
Message
MessageHandler::ProcessJoin(const Message& message) {
  // message to send "Please enter a room code."
  // accepted inputs: room codes
  return Message{message.connection, "inside proccess join.\n"};
}

Message
MessageHandler::ProcessCreate(const Message& message) {
  // message to send: "Choose a game to play."
  // accepted inputs: list of games
  return Message{message.connection, "inside proccess create.\n"};
}

// MessageResult
// MessageHandler::processMessages(std::unique_ptr<Server> server, const std::deque<Message>& incoming) {
//   std::ostringstream result;
//   bool quit = false;
//   for (const auto& message : incoming) {
//     if (message.text == "quit") {
//       server->disconnect(message.connection);
//     } else if (message.text == "shutdown") {
//       std::cout << "Shutting down.\n";
//       quit = true;
//     } else {
//       result << message.connection.id << "> " << message.text << "\n";
//     }
//   }
//   return MessageResult{result.str(), quit};
// }


std::deque<Message>
MessageHandler::buildOutgoing(const std::string_view& log) {
    std::deque<Message> outgoing;
    return outgoing;
}