#include "UserManager.h"

#include <iostream>

void 
UserManager::addUser(Connection userID) {
    auto it = getUserItrByID(userID);

    if (it == users.end()) {
        users.emplace_back(User{userID});
    } else {
        throw std::runtime_error("User already exists!");
    }
}

void 
UserManager::removeUser(Connection userID) {
    auto it = std::remove_if(users.begin(), users.end(), [userID] (const User& user) { 
        return user.userID == userID; 
    });
    users.erase(it, users.end());
}

void 
UserManager::setUserName(Connection userID, std::string_view username) {
    auto it = getUserItrByID(userID);
    it->username = username;
}

void 
UserManager::setUserRole(Connection userID, Role role) {
    auto it = getUserItrByID(userID);    
    it->role = role;
}

void
UserManager::setUserRoomCode(Connection userID, uint16_t roomCode) {
    auto it = getUserItrByID(userID);
    it->roomCode = roomCode;
}

void 
UserManager::setUserState(Connection userID, UserState state) {
    auto it = getUserItrByID(userID);
    it->state = state;
}

std::vector<User> 
UserManager::getAllUsers() const { 
    return users; 
}

std::vector<User>
UserManager::getUsersInGame(uint16_t userRoomCode) const {
    std::vector<User> usersInGame;
    std::copy_if(users.begin(), users.end(), std::back_inserter(usersInGame), [userRoomCode] (const User& user) {
        return user.roomCode == userRoomCode;
    });
    return usersInGame;
}

User 
UserManager::getRoomOwner(uint16_t roomCode) const {
    auto it = std::find_if(users.begin(), users.end(), [roomCode](const User& user) {
        return user.roomCode == roomCode && user.role == Role::OWNER;
    });
    return *it;
}

User
UserManager::getUserByID(Connection userID) const {
    auto it = std::find_if(users.begin(), users.end(), [userID](const User& user) {
        return user.userID == userID;
    });
    return *it;
}

std::vector<User>::iterator 
UserManager::getUserItrByID(Connection userID) {
    return std::find_if(users.begin(), users.end(), [userID](const User& user) {
        return user.userID == userID;
    });
}
