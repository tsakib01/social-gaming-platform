#include "InGameUserManager.h"
#include <cassert>

    void InGameUserManager::addNewPlayer(uint32_t userID, Environment playerStates){
        InGameUserManager::m_playerStates.insert({userID, playerStates});
    }

    void InGameUserManager::addNewAudience(uint32_t userID, Environment audienceStates){
        InGameUserManager::m_audienceStates.insert({userID, audienceStates});
    }

    void InGameUserManager::deletePlayer(uint32_t userID){
        InGameUserManager::m_playerStates.erase({userID});
    }

    void InGameUserManager::deleteAudience(uint32_t userID){
        InGameUserManager::m_audienceStates.erase({userID});
    }

    // Get a specific user's states from their corresponding map.
    std::map<uint32_t, Environment>::iterator InGameUserManager::getStatesOfPlayer(uint32_t userID){
        std::map<uint32_t, Environment>::iterator m_getStatesOfAllPlayersIterator;
        m_getStatesOfAllPlayersIterator = m_playerStates.find(userID);

        // It should never happen that we try looking for a player that doesn't exist.
        // But this may prove useful when, for example, we are trying to make sure this class
        // and others are all keeping track of a user having been deleted.
        assert(m_getStatesOfAllPlayersIterator != m_playerStates.end());
        return m_getStatesOfAllPlayersIterator;
    }  

    std::map<uint32_t, Environment>::iterator InGameUserManager::getStatesOfAudience(uint32_t userID){
        std::map<uint32_t, Environment>::iterator m_getStatesOfAllAudiencesIterator;
        m_getStatesOfAllAudiencesIterator = m_audienceStates.find(userID);
        assert(m_getStatesOfAllAudiencesIterator != m_audienceStates.end());
        return m_getStatesOfAllAudiencesIterator;
    }
    
    std::map<uint32_t, Environment> InGameUserManager::getStatesOfAllPlayers(){
        return m_playerStates;
    }

    std::map<uint32_t, Environment> InGameUserManager::getStatesOfAllAudiences(){
        return m_audienceStates;
    }