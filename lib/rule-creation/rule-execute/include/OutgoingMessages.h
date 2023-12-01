#pragma once

#include "UserManager.h"
#include <map>

/// Representation of messages that should be sent as a result of executing a rule
class OutgoingMessages {
public:
    OutgoingMessages(std::vector<UserId> users);
    
    void setMessage(UserId user, std::string_view message);
    void setMessageForAll(std::string_view message);
    std::map<UserId, std::string_view> getMessages() const;
    /// Clears all messages but retains the users
    void clear();

private:
    std::map<UserId, std::string_view> m_userMessages;
};
