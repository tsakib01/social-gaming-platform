#include "InGameUserManager.h"
#include "Server.h"
#include <cassert>

    void InGameUserManager::addNewUser(intptr_t userID, Role role, GameEnvironment::Environment userStates){
        auto iterator = m_userStates.find(userID);

        // // User should not already exist in the game.
        assert(iterator == m_userStates.end());
        std::pair<Role, GameEnvironment::Environment> userStartingStates;
        userStartingStates.first = role;
        userStartingStates.second = std::move(userStates);
        InGameUserManager::m_userStates.insert({userID, std::move(userStartingStates)});
    }

    void InGameUserManager::deleteUser(intptr_t userID){
        // erase returns 0 if something was not erased.
        // If assert failed, that means the user already doesn't exist in this game
        // which should never happen if this is called.
        assert(std::move(InGameUserManager::m_userStates).erase({userID}) != 0); 
    }

    // Ref: https://www.javatpoint.com/post/cpp-map-find-function
    GameEnvironment::Environment InGameUserManager::getStatesOfUser(intptr_t userID){
        auto iterator = m_userStates.find(userID);
        assert(iterator != m_userStates.end());
        std::pair<Role, GameEnvironment::Environment> statesToGet = std::move(iterator -> second);
        return std::move(statesToGet.second);
    }

    std::map<intptr_t, std::pair<Role, GameEnvironment::Environment>> InGameUserManager::getAllUserStates(){
        return std::move(InGameUserManager::m_userStates);
    }

    // // Recall that m_userStates is of type std::map<uint32_t, std::map<Role, Environment>>
    // // Create a new pair to replace the existing one that the User ID maps to.
    void InGameUserManager::setStatesOfUser(intptr_t userID, Role role, GameEnvironment::Environment states){
        auto iterator = m_userStates.find(userID);
        // User SHOULD already exist in the game.
        assert(iterator != m_userStates.end());
        std::pair<Role, GameEnvironment::Environment> statesToSet;
        statesToSet.first = role;
        std::move(statesToSet.second) = std::move(states);

        //InGameUserManager::m_userStates.insert({userID, statesToSet});
        // insert() doesn't work if the key (the userID in this case) already exists
        // in the map, so use the line below instead to replace what the userID is mapped to.
        InGameUserManager::m_userStates[userID] = std::move(statesToSet);
    }