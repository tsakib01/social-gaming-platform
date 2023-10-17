#include "UserManager.h"

#include <iostream>

void 
UserManager::addUser(uint32_t userID, std::string_view username) {
    auto iterator = std::find_if(users.begin(), users.end(), 
                                [userID] (const User& user) { return user.userID == userID; });

    if(iterator == users.end()) { users.emplace_back(User{userID, username, Role::NONE, 0}); }
    else std::cout << "User with " << userID << " already exists." << std::endl;
}

void 
UserManager::setRole(uint32_t userID, Role role) {
    auto iterator = std::find_if(users.begin(), users.end(), 
                                [userID] (const User& user) { return user.userID == userID; });
    
    if(iterator != users.end()) { iterator->role = role; }
    else std::cout << "User with " << userID << " doesn't exist." << std::endl;
}

void 
UserManager::setRoom(uint32_t userID, uint8_t roomCode) {
    auto iterator = std::find_if(users.begin(), users.end(), 
                                [userID] (const User& user) { return user.userID == userID; });
    
    if(iterator != users.end()) { iterator->roomCode = roomCode; }
    else std::cout << "User with " << userID << " doesn't exist." << std::endl;
}

void 
UserManager::removeUser(uint32_t userID) {
    auto iterator = std::remove_if(users.begin(), users.end(), 
                                    [userID] (const User& user) { return user.userID == userID; });

    users.erase(iterator, users.end());
}

