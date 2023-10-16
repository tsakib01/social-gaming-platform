#include "UserManager.h"

#include <iostream>

void 
UserManager::addUser(uint32_t userID, std::string username) {
    auto iterator = std::find_if(unassignedUsers.begin(), unassignedUsers.end(), 
                                [userID] (const User& user) { return user.userID == userID; });

    if(iterator == unassignedUsers.end()) { unassignedUsers.emplace_back(User{userID, username, ""}); }
    else std::cout << "User with " << userID << " already exists." << std::endl;
}

void 
UserManager::setRole(uint32_t userID, std::string role) {
    auto iterator = std::find_if(unassignedUsers.begin(), unassignedUsers.end(), 
                                [userID] (const User& user) { return user.userID == userID; });
    
    if(iterator != unassignedUsers.end()) { iterator->role = role; }
    else std::cout << "User with " << userID << " doesn't exist." << std::endl;
}

void 
UserManager::addToRoom(uint32_t userID, uint8_t roomCode) {
    auto iterator = std::find_if(unassignedUsers.begin(), unassignedUsers.end(), 
                                [userID] (const User& user) { return user.userID == userID; });
    
    if(iterator != unassignedUsers.end()) {
        users[roomCode].emplace_back(*iterator); 
        std::iter_swap(iterator, unassignedUsers.end() - 1);  
        unassignedUsers.pop_back();  
    }
    else std::cout << "User with " << userID << " doesn't exist." << std::endl;
}

