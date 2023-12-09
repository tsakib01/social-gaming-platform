#include "InGameUserManager.h"
#include "Server.h"
#include <cassert>
#include <iostream>

void InGameUserManager::addNewUser(UserId connection, GameEnvironment::Environment userStates){
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

std::map<uintptr_t, GameEnvironment::Environment> InGameUserManager::getAllUserStates(){
    return std::move(m_userStates);
}

std::vector<UserId> InGameUserManager::getAllUserIds() const {
    auto userIds = std::vector<UserId>{};
    std::transform(m_userStates.begin(), m_userStates.end(), std::back_inserter(userIds),
        [](const auto& userState) { return UserId{userState.first}; });
    return userIds;
}

GameEnvironment::Value InGameUserManager::getValueOfUser(UserId connection, GameEnvironment::Identifier identifier){
    auto iterator = m_userStates.find(connection.id);
    assert(iterator != m_userStates.end());
    GameEnvironment::Environment& environment = m_userStates[connection.id];
    // Value is stored a unique_ptr, so we should use * to dereference it
    return *environment[identifier];
}

void InGameUserManager::setIdentifierOfUser(
    UserId connection, GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value> value){
    auto usersIterator = m_userStates.find(connection.id);
    assert(usersIterator != m_userStates.end());
    // Get the environment from the correct User ID
    GameEnvironment::Environment& environment = m_userStates[connection.id];
    
    // Set the environment's new value to the identifier
    environment[identifier] = std::move(value);
}