#include "InGameUserManager.h"
#include <cassert>

    // Assume the role is passed as an Enum (stored within UserManager).
    void InGameUserManager::addNewUser(uint32_t userID, Role role, Environment userStates){
        // Have to make a new map which will be inserted as the value.
        // Might it be possible for Role and Environment to be passed in the arguments as a map?
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
    Environment InGameUserManager::getStatesOfUser(uint32_t userID, Role role){
        auto iterator = m_userStates.find(userID);
        assert(iterator != m_userStates.end());
        std::pair<Role, Environment> statesToGet = iterator -> second;
        return statesToGet.second;
    }
    
    // Recall that m_userStates is of type std::map<uint32_t, std::map<Role, Environment>>
    // Create a new map to replace the existing one that the User ID maps to.
    void InGameUserManager::setStatesOfUser(uint32_t userID, Role role, Environment states){
        std::pair<Role, Environment> statesToSet;
        statesToSet.first = role;
        statesToSet.second = states;
        InGameUserManager::m_userStates.insert({userID, statesToSet});
    }