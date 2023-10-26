#include "UserManager.h"

#include <iostream>

void 
UserManager::addUser(Connection userID) {
    auto it = std::find_if(users.begin(), users.end(), [userID](const User& user) {
        return user.userID == userID;
    });

    if (it == users.end()) {
        users.emplace_back(User{userID});
    } else {
        throw std::runtime_error("Adding a user that already exists.");
    }
}

void 
UserManager::setUserName(Connection userID, std::string_view username) {
    auto it = std::find_if(users.begin(), users.end(), [userID](const User& user) {
        return user.userID == userID;
    });

    it->username = username;
}

void 
UserManager::setUserRole(Connection userID, Role role) {
    auto it = std::find_if(users.begin(), users.end(), [userID] (const User& user) { 
        return user.userID == userID; 
    });
    
    it->role = role;
}

void
UserManager::setUserRoomCode(Connection userID, uint8_t roomCode) {
    auto it = std::find_if(users.begin(), users.end(), [userID](const User& user) {
        return user.userID == userID;
    });

    it->roomCode = roomCode;
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
    
    uint8_t userRoomCode = getUserGameCode(userID);
    std::copy_if(users.begin(), users.end(), std::back_inserter(usersInGame), [userRoomCode] (const User& user) {
        return user.roomCode == userRoomCode;
    });
    
    return usersInGame;
}

uint8_t
UserManager::getUserGameCode(Connection userID) {
    auto it = std::find_if(users.begin(), users.end(), [userID](const User& user) {
        return user.userID == userID;
    });

    return it->roomCode;
}