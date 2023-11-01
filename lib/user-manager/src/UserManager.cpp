#include "UserManager.h"

#include <iostream>

void 
UserManager::addUser(Connection userID) {
    auto it = findUserByID(userID);

    if (it == users.end()) {
        users.emplace_back(std::make_shared<User>(userID));
    } else {
        throw std::runtime_error("User already exists!");
    }
}

void 
UserManager::setUserName(Connection userID, std::string_view username) {
    auto it = findUserByID(userID);
    (*it)->username = username;
}

void 
UserManager::setUserRole(Connection userID, Role role) {
    auto it = findUserByID(userID);
    (*it)->role = role;
}

void
UserManager::setUserRoomCode(Connection userID, uint16_t roomCode) {
    auto it = findUserByID(userID);
    (*it)->roomCode = roomCode;
}

void 
UserManager::removeUser(Connection userID) {
    auto it = std::remove_if(users.begin(), users.end(), [userID] (const std::shared_ptr<User>& user) { 
        return user->userID == userID; 
    });
    users.erase(it, users.end());
}

std::vector<std::shared_ptr<User>>
UserManager::getUsersInGame(Connection userID) {
    std::vector<std::shared_ptr<User>> usersInGame;
    
    uint16_t userRoomCode = getUserRoomCode(userID);
    std::copy_if(users.begin(), users.end(), std::back_inserter(usersInGame), [userRoomCode] (const std::shared_ptr<User>& user) {
        return user->roomCode == userRoomCode;
    });
    
    return usersInGame;
}

uint16_t
UserManager::getUserRoomCode(Connection userID) {
    auto it = findUserByID(userID);
    return (*it)->roomCode;
}

std::vector<std::shared_ptr<User>>::iterator 
UserManager::findUserByID(Connection userID) {
    return std::find_if(users.begin(), users.end(), [userID](const std::shared_ptr<User>& user) {
        return user->userID == userID;
    });
}