#include "UserManager.h"

#include <iostream>

void 
UserManager::addUser(Connection userID) {
    auto it = findUserByID(userID);

    if (it == users.end()) {
        users.emplace_back(User{userID});
    } else {
        throw std::runtime_error("User already exists!");
    }
}

void 
UserManager::setUserName(Connection userID, std::string_view username) {
    auto it = findUserByID(userID);
    it->username = username;
}

void 
UserManager::setUserRole(Connection userID, Role role) {
    auto it = findUserByID(userID);    
    it->role = role;
}

void
UserManager::setUserRoomCode(Connection userID, uint16_t roomCode) {
    auto it = findUserByID(userID);
    it->roomCode = roomCode;
}

void 
UserManager::setUserState(Connection userID, UserState state) {
    auto it = findUserByID(userID);
    it->state = state;
}

void 
UserManager::removeUser(Connection userID) {
    auto it = std::remove_if(users.begin(), users.end(), [userID] (const User& user) { 
        return user.userID == userID; 
    });
    users.erase(it, users.end());
}

std::vector<User>
UserManager::getUsersInGame(Connection userID) {
    std::vector<User> usersInGame;
    
    uint16_t userRoomCode = getUserGameCode(userID);
    std::copy_if(users.begin(), users.end(), std::back_inserter(usersInGame), [userRoomCode] (const User& user) {
        return user.roomCode == userRoomCode;
    });
    
    return usersInGame;
}

std::vector<User> 
UserManager::getUsersByRoomCode(uint16_t roomCode) {
    std::vector<User> usersInRoom;

    std::copy_if(users.begin(), users.end(), std::back_inserter(usersInRoom), [roomCode] (const User& user) {
        return user.roomCode == roomCode;
    });
    
    return usersInRoom;
}

uint16_t
UserManager::getUserGameCode(Connection userID) {
    auto it = findUserByID(userID);
    return it->roomCode;
}

std::vector<User>::iterator 
UserManager::findUserByID(Connection userID) {
    return std::find_if(users.begin(), users.end(), [userID](const User& user) {
        return user.userID == userID;
    });
}