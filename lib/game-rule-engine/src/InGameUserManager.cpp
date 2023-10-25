#include "InGameUserManager.h"
#include <cassert>

    void InGameUserManager::addNewPlayer(uint32_t userID, Environment playerStates){
        InGameUserManager::m_playerStates.insert({userID, playerStates});
    }

    void InGameUserManager::addNewAudience(uint32_t userID, Environment audienceStates){
        InGameUserManager::m_audienceStates.insert({userID, audienceStates});
    }

    void InGameUserManager::deleteUser(uint32_t userID){
        // erase returns 0 if something was not erased.
        // If assert failed, that means the user already doesn't exist in this game
        // which should never happen if this is called.
        assert(InGameUserManager::m_playerStates.erase({userID}) != 0 && 
        InGameUserManager::m_audienceStates.erase({userID}) != 0);
    }

    void InGameUserManager::setStatesOfUser(uint32_t userID, Environment states){
        std::map<uint32_t, Environment>::iterator m_getStatesOfAllPlayersIterator;
        std::map<uint32_t, Environment>::iterator m_getStatesOfAllAudiencesIterator;
        m_getStatesOfAllPlayersIterator = m_playerStates.find(userID);
        m_getStatesOfAllAudiencesIterator = m_audienceStates.find(userID);
        assert(m_getStatesOfAllPlayersIterator != m_playerStates.end()
        || m_getStatesOfAllAudiencesIterator != m_audienceStates.end());

        if (m_getStatesOfAllPlayersIterator != m_playerStates.end()){
            m_playerStates[userID] = states;
        } else {
            m_audienceStates[userID] = states;
        }
    }

    Environment InGameUserManager::getStatesOfUser(uint32_t userID){
        std::map<uint32_t, Environment>::iterator m_getStatesOfAllPlayersIterator;
        std::map<uint32_t, Environment>::iterator m_getStatesOfAllAudiencesIterator;
        m_getStatesOfAllPlayersIterator = m_playerStates.find(userID);
        m_getStatesOfAllAudiencesIterator = m_audienceStates.find(userID);
        assert(m_getStatesOfAllPlayersIterator != m_playerStates.end()
        || m_getStatesOfAllAudiencesIterator != m_audienceStates.end());

        // Instead of returning a whole iterator, return the second value which is just the states.
        if(m_getStatesOfAllPlayersIterator != m_playerStates.end()){
            return m_getStatesOfAllPlayersIterator->second;
        }
        return m_getStatesOfAllAudiencesIterator->second;
    }
    
    std::map<uint32_t, Environment> InGameUserManager::getStatesOfAllPlayers(){
        return m_playerStates;
    }

    std::map<uint32_t, Environment> InGameUserManager::getStatesOfAllAudiences(){
        return m_audienceStates;
    }