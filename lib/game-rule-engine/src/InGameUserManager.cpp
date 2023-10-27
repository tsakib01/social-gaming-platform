#include "InGameUserManager.h"
#include <cassert>

    // Assume the role is passed as an Enum (stored within UserManager).
    void InGameUserManager::addNewUser(uint32_t userID, Role role, Environment userStates){
        // Have to make a new map which will be inserted as the value.
        // Might it be possible for Role and Environment to be passed in the arguments as a map?
        std::map<Role, Environment> userStartingStates;
        userStartingStates.insert({role, userStates});
        InGameUserManager::m_userStates.insert({userID, userStartingStates});
    }

    void InGameUserManager::deleteUser(uint32_t userID){
        // erase returns 0 if something was not erased.
        // If assert failed, that means the user already doesn't exist in this game
        // which should never happen if this is called.
        assert(InGameUserManager::m_userStates.erase({userID}) != 0); 
    }

    Environment InGameUserManager::getStatesOfUser(uint32_t userID, Role role){
        std::map<uint32_t, std::map<Role, Environment>>::iterator statesIteratorByID;
        std::map<Role, Environment>::iterator statesIteratorByRole;
        statesIteratorByID = m_userStates.find(userID);
        assert(statesIteratorByID != m_userStates.end());
        std::map<Role, Environment> statesToGet = statesIteratorByID -> second;
        statesIteratorByRole = statesToGet.find(role);
        assert(statesIteratorByRole != statesToGet.end());
        return statesIteratorByRole -> second;
    }
    
    // Recall that m_userStates is of type std::map<uint32_t, std::map<Role, Environment>>
    // Create a new map to replace the existing one that the User ID maps to.
    void InGameUserManager::setStatesOfUser(uint32_t userID, Role role, Environment states){
        std::map<Role, Environment> statesToSet;
        statesToSet.insert({role, states});
        m_userStates[userID] = statesToSet;
    }