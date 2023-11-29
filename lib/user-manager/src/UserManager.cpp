#include "UserManager.h"

#include <iostream>

void 
UserManager::addUser(Connection userID) {
    auto it = std::find_if(m_users.begin(), m_users.end(), [userID](const User& user) {
        return user.userID == userID;
    });

    if (it == m_users.end()) {
        m_users.emplace_back(User{userID});
    } else {
        throw std::runtime_error("User already exists!");
    }
}

void 
UserManager::removeUser(Connection userID) {
    auto it = std::remove_if(m_users.begin(), m_users.end(), [userID] (const User& user) { 
        return user.userID == userID; 
    });
    m_users.erase(it, m_users.end());
}

void 
UserManager::setUserName(Connection userID, std::string_view username) {
    auto& user = getUserReferenceByID(userID);
    user.username = username;
}

void 
UserManager::setUserRole(Connection userID, Role role) {
    auto& user = getUserReferenceByID(userID);    
    user.role = role;
}

void
UserManager::setUserRoomCode(Connection userID, uint16_t roomCode) {
    auto& user = getUserReferenceByID(userID);
    user.roomCode = roomCode;
}

void 
UserManager::setUserState(Connection userID, UserState state) {
    auto& user = getUserReferenceByID(userID);
    user.state = state;
}

std::vector<User> 
UserManager::getAllUsers() const { 
    return m_users; 
}

std::vector<User>
UserManager::getUsersInGame(uint16_t userRoomCode) const {
    std::vector<User> usersInGame;
    std::copy_if(m_users.begin(), m_users.end(), std::back_inserter(usersInGame), [userRoomCode] (const User& user) {
        return user.roomCode == userRoomCode;
    });
    return usersInGame;
}

User 
UserManager::getRoomOwner(uint16_t roomCode) const {
    auto it = std::find_if(m_users.begin(), m_users.end(), [roomCode](const User& user) {
        return user.roomCode == roomCode && user.role == Role::OWNER;
    });
    return *it;
}

User
UserManager::getUserByID(Connection userID) const {
    auto it = std::find_if(m_users.begin(), m_users.end(), [userID](const User& user) {
        return user.userID == userID;
    });
    return *it;
}

User&
UserManager::getUserReferenceByID(Connection userID) {
    auto it = std::find_if(m_users.begin(), m_users.end(), [userID](const User& user) {
        return user.userID == userID;
    });

    if (it == m_users.end()) {
        throw std::runtime_error("User was not found.");
    }

    return *it;
}
