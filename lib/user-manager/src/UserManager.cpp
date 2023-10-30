#include "UserManager.h"

#include <iostream>

bool 
UserManager::addUser(Connection userID) {
    auto it = findUserByID(userID);

    if (it == users.end())
        users.emplace_back(User{userID});
    else
        return false;

    return true;
}

bool 
UserManager::setUserName(Connection userID, std::string_view username) {
    auto it = findUserByID(userID);
    if (it == users.end()) return false;
    it->username = username;
    return true;
}

bool 
UserManager::setUserRole(Connection userID, Role role) {
    auto it = findUserByID(userID);    
    if (it == users.end()) return false;
    it->role = role;
    return true;
}

bool
UserManager::setUserRoomCode(Connection userID, uint16_t roomCode) {
    auto it = findUserByID(userID);
    if (it == users.end()) return false;
    it->roomCode = roomCode;
    return true;
}

bool 
UserManager::removeUser(Connection userID) {
    auto it = std::remove_if(users.begin(), users.end(), [userID] (const User& user) { 
        return user.userID == userID; 
    });
    if (it == users.end()) return false;
    users.erase(it, users.end());
    return true;
}

std::vector<User>
UserManager::getUsersInGame(Connection userID) {
    std::vector<User> usersInGame;
    
    uint16_t userRoomCode = getUserRoomCode(userID);
    std::copy_if(users.begin(), users.end(), std::back_inserter(usersInGame), [userRoomCode] (const User& user) {
        return user.roomCode == userRoomCode;
    });
    
    return usersInGame;
}

uint16_t
UserManager::getUserRoomCode(Connection userID) {
    auto it = findUserByID(userID);
    return it->roomCode;
}

std::vector<User>::iterator 
UserManager::findUserByID(Connection userID) {
    return std::find_if(users.begin(), users.end(), [userID](const User& user) {
        return user.userID == userID;
    });
}