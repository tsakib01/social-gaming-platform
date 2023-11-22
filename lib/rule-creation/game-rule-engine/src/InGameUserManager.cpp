#include "InGameUserManager.h"
#include "Server.h"
#include <cassert>

#include <iostream> // ALSO DELETE THIS
    
    void InGameUserManager::addNewUser(networking::Connection connection, GameEnvironment::Environment userStates){
        auto iterator = m_userStates.find(connection.id);

        // DELETE THIS RIGHT AFTER 
        std::cout << "connection : " << connection.id << std::endl;

        // User should not already exist in the game.
        assert(iterator == m_userStates.end());

        m_userStates.insert({connection.id, std::move(userStates)});
    }

    void InGameUserManager::deleteUser(networking::Connection connection){
        // erase returns 0 if something was not erased.
        // If assert failed, that means the user already doesn't exist in this game
        // which should never happen if this is called.
        assert(std::move(InGameUserManager::m_userStates).erase(connection.id) != 0); 
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