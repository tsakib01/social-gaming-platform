#include "InGameUserManager.h"
#include <cassert>

    // Assume the role is passed as an Enum (stored within UserManager).
    void InGameUserManager::addNewUser(uint32_t userID, Role role, Environment userStates){
        auto iterator = m_userStates.find(userID);

        // User should not already exist in the game.
        assert(iterator == m_userStates.end());
        std::pair<Role, Environment> userStartingStates;
        userStartingStates.first = role;
        userStartingStates.second = userStates;
        InGameUserManager::m_userStates.insert({userID, userStartingStates});
    }

    void InGameUserManager::deleteUser(uint32_t userID){
        // erase returns 0 if something was not erased.
        // If assert failed, that means the user already doesn't exist in this game
        // which should never happen if this is called.
        assert(InGameUserManager::m_userStates.erase({userID}) != 0); 
    }

    // Ref: https://www.javatpoint.com/post/cpp-map-find-function
    Environment InGameUserManager::getStatesOfUser(uint32_t userID){
        auto iterator = m_userStates.find(userID);
        assert(iterator != m_userStates.end());
        std::pair<Role, Environment> statesToGet = iterator -> second;
        return statesToGet.second;
    }

    std::map<uint32_t, std::pair<Role, Environment>> InGameUserManager::getAllUserStates(){
        return InGameUserManager::m_userStates;
    }

    // Recall that m_userStates is of type std::map<uint32_t, std::map<Role, Environment>>
    // Create a new pair to replace the existing one that the User ID maps to.
    void InGameUserManager::setStatesOfUser(uint32_t userID, Role role, Environment states){
        auto iterator = m_userStates.find(userID);
        // User SHOULD already exist in the game.
        assert(iterator != m_userStates.end());
        std::pair<Role, Environment> statesToSet;
        statesToSet.first = role;
        statesToSet.second = states;

        //InGameUserManager::m_userStates.insert({userID, statesToSet});
        // insert() doesn't work if the key (the userID in this case) already exists
        // in the map, so use the line below instead to replace what the userID is mapped to.
        InGameUserManager::m_userStates[userID] = statesToSet;
    }