#include "InGameUserManager.h"
#include <cassert>

    void InGameUserManager::addNewPlayer(int userID, Environment playerStates){
        InGameUserManager::m_playerStates.insert({userID, playerStates});
    }

    void InGameUserManager::addNewAudience(int userID, Environment audienceStates){
        InGameUserManager::m_audienceStates.insert({userID, audienceStates});
    }

    void InGameUserManager::deletePlayer(int userID){
        InGameUserManager::m_playerStates.erase({userID});
    }

    void InGameUserManager::deleteAudience(int userID){
        InGameUserManager::m_audienceStates.erase({userID});
    }

    // Get a specific user's states from their corresponding map.
    std::map<int, Environment>::iterator InGameUserManager::getStatesOfPlayer(int userID){
        std::map<int, Environment>::iterator m_getStatesOfAllPlayersIterator;
        m_getStatesOfAllPlayersIterator = m_playerStates.find(userID);

        // It should never happen that we try looking for a player that doesn't exist.
        // But this may prove useful when, for example, we are trying to make sure this class
        // and others are all keeping track of a user having been deleted.
        assert(m_getStatesOfAllPlayersIterator != m_playerStates.end());
        return m_getStatesOfAllPlayersIterator;
    }  

    std::map<int, Environment>::iterator InGameUserManager::getStatesOfAudience(int userID){
        std::map<int, Environment>::iterator m_getStatesOfAllAudiencesIterator;
        m_getStatesOfAllAudiencesIterator = m_audienceStates.find(userID);
        assert(m_getStatesOfAllAudiencesIterator != m_audienceStates.end());
        return m_getStatesOfAllAudiencesIterator;
    }
    
    std::map<int, Environment> InGameUserManager::getStatesOfAllPlayers(){
        return m_playerStates;
    }

    std::map<int, Environment> InGameUserManager::getStatesOfAllAudiences(){
        return m_audienceStates;
    }