#include "UserManager.h"

#include <iostream>

void 
UserManager::addUser(Connection userID) {
    auto it = getUserByID(userID);

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
    auto it = getUserByID(userID);
    it->username = username;
}

void 
UserManager::setUserRole(Connection userID, Role role) {
    auto it = getUserByID(userID);    
    it->role = role;
}

void
UserManager::setUserRoomCode(Connection userID, uint16_t roomCode) {
    auto it = getUserByID(userID);
    it->roomCode = roomCode;
}

void 
UserManager::setUserState(Connection userID, UserState state) {
    auto it = getUserByID(userID);
    it->state = state;
}

std::vector<User> 
UserManager::getAllUsers() const { 
    return users; 
}

std::vector<User>
UserManager::getUsersInGame(uint16_t userRoomCode) {
    std::vector<User> usersInGame;
    std::copy_if(users.begin(), users.end(), std::back_inserter(usersInGame), [userRoomCode] (const User& user) {
        return user.roomCode == userRoomCode;
    });
    
    return usersInGame;
}

std::vector<User>::iterator 
UserManager::getUserByID(Connection userID) {
    return std::find_if(users.begin(), users.end(), [userID](const User& user) {
        return user.userID == userID;
    });
}

std::vector<User>::iterator 
UserManager::getRoomOwner(uint16_t roomCode) {
    return std::find_if(users.begin(), users.end(), [roomCode](const User& user) {
        return user.roomCode == roomCode && user.role == Role::OWNER;
    });
}