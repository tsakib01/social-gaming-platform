#include "InGameUserManager.h"
#include "Server.h"
#include <cassert>
#include <iostream>

void InGameUserManager::addNewUser(UserId connection, GameEnvironment::Environment userStates){

    // Checking that key-value mappings are are reachable
    // std::unique_ptr<GameEnvironment::Value> value = std::move(userStates["testidentifier"]);
    // GameEnvironment::Value* theVal = value.get();
    // if(value == nullptr){
    //     std::cout << "Value's unique pointer null!" << "\n";
    // }
    
    // if(theVal == nullptr){
    //     std::cout << "Value's pointer is null!!" << "\n";
    // }

    // The size returned at the moment is 0. Why?
    // std::cout << "m_userStates.size() is " << m_userStates.size() << '\n';
    auto iterator = m_userStates.find(connection.id);

    // User should not already exist in the game.
    assert(iterator == m_userStates.end());

    m_userStates.insert({connection.id, std::move(userStates)});
}

void InGameUserManager::deleteUser(UserId connection){
    auto iterator = m_userStates.find(connection.id);

    // If assert failed, that means the user already doesn't exist in this game
    // which should never happen if this is called.
    assert(iterator != m_userStates.end());
    m_userStates.erase(iterator);
}

// Ref: https://www.javatpoint.com/post/cpp-map-find-function
GameEnvironment::Environment& InGameUserManager::getStatesOfUser(UserId connection){
    auto iterator = m_userStates.find(connection.id);
    
    assert(iterator != m_userStates.end());
    GameEnvironment::Environment& statesToGet = m_userStates[connection.id];
    return statesToGet;
}

std::map<uintptr_t, GameEnvironment::Environment> InGameUserManager::getAllUserStates(){
    return std::move(m_userStates);
}

// Create a new pair to replace the existing one that the User ID maps to.
void InGameUserManager::setStatesOfUser(UserId userID, GameEnvironment::Environment statesToSet){
    auto iterator = m_userStates.find(userID.id);

    // User SHOULD already exist in the game.
    assert(iterator != m_userStates.end());

    // insert() doesn't work when the key (the userID in this case) already exists
    // in the map, so use the line below instead to replace what the userID is mapped to.
    InGameUserManager::m_userStates[userID.id] = std::move(statesToSet);
}