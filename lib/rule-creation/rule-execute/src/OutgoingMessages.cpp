#include "OutgoingMessages.h"
#include <iostream>
OutgoingMessages::OutgoingMessages(std::vector<UserId> users) {
    auto inserter = std::inserter(m_userMessages, m_userMessages.end());
    auto makeEmptyMsg = [] (const auto& user) { return std::make_pair(user, ""); };
    std::transform(users.begin(), users.end(), inserter, makeEmptyMsg);
}

void OutgoingMessages::setMessage(UserId user, std::string_view message) {
    if(m_userMessages.find(user) == m_userMessages.end())
        throw std::runtime_error("User does not exist");
    
    m_userMessages[user] = message;
}

void OutgoingMessages::setMessageForAll(std::string_view message) {
    for(auto& [user, _] : m_userMessages)
        setMessage(user, message);
}

std::map<UserId, std::string_view>
OutgoingMessages::getMessages() const {
    return m_userMessages;
}

void OutgoingMessages::clear() {
    for(auto& [user, _] : m_userMessages)
        setMessage(user, "");
}
