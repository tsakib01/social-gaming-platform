#include "InGameUserManager.h"
#include "Server.h"
#include <cassert>
#include <iostream>

void InGameUserManager::addNewUser(networking::Connection connection, GameEnvironment::Environment userStates){
    std::cout<< "Does anything happen here?" << "\n";

    //connection.id is passed to this function successfully
    std::cout << "THE ID HERE IS" << connection.id << "\n";
    if( &userStates == NULL){
        std::cout << "Address is null" << "\n";
    }

    std::unique_ptr<GameEnvironment::Value> value = std::move(userStates["testidentifier"]);
    GameEnvironment::Value* theVal = value.get();
    if(value == nullptr){
        std::cout << "Value's unique pointer null!" << "\n";
    }
    
    if(theVal == nullptr){
        std::cout << "Value's pointer is null!!" << "\n";
    }

    auto iterator = m_userStates.find(connection.id);

    // // User should not already exist in the game.
    assert(iterator == m_userStates.end());

    m_userStates.insert({connection.id, std::move(userStates)});
}

void InGameUserManager::deleteUser(networking::Connection connection){
    std::cout << "THE ID TO DELETE HERE IS" << connection.id << "\n";
    // erase returns 0 if something was not erased.
    // If assert failed, that means the user already doesn't exist in this game
    // which should never happen if this is called.
    //assert(m_userStates.erase(connection.id) != 0); 
    assert(m_userStates.erase(connection.id) != 0);
}

// Ref: https://www.javatpoint.com/post/cpp-map-find-function
GameEnvironment::Environment InGameUserManager::getStatesOfUser(networking::Connection connection){
    auto iterator = m_userStates.find(connection.id);
    
    assert(iterator != m_userStates.end());
    GameEnvironment::Environment statesToGet = std::move(m_userStates[connection.id]);
    return statesToGet;
}

std::map<uintptr_t, GameEnvironment::Environment> InGameUserManager::getAllUserStates(){
    return std::move(InGameUserManager::m_userStates);
}

// Recall that m_userStates is of type std::map<uint32_t, std::map<Role, Environment>>
// Create a new pair to replace the existing one that the User ID maps to.
void InGameUserManager::setStatesOfUser(networking::Connection userID, GameEnvironment::Environment statesToSet){
    auto iterator = m_userStates.find(userID.id);

    // User SHOULD already exist in the game.
    assert(iterator != m_userStates.end());

    // insert() doesn't work when the key (the userID in this case) already exists
    // in the map, so use the line below instead to replace what the userID is mapped to.
    InGameUserManager::m_userStates[userID.id] = std::move(statesToSet);
}