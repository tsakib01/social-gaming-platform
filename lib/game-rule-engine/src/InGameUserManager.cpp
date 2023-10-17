#include "InGameUserManager.h"

    void InGameUserManager::addNewPlayer(int userID, std::vector<Environment> playerStates){
        InGameUserManager::m_playerStates.insert({userID, playerStates});
    }

    void InGameUserManager::addNewAudience(int userID, std::vector<Environment> audienceStates){
        InGameUserManager::m_audienceStates.insert({userID, audienceStates});
    }

    void InGameUserManager::deletePlayer(int userID){
        InGameUserManager::m_playerStates.erase({userID});
    }

    void InGameUserManager::deleteAudience(int userID){
        InGameUserManager::m_audienceStates.erase({userID});
    }

    // Get a specific user's states from their corresponding map.
    std::map<int, std::vector<Environment>>::iterator InGameUserManager::getStatesOfPlayer(int userID){
        std::map<int, std::vector<Environment>>::iterator m_getStatesOfAllPlayersIterator;
        m_getStatesOfAllPlayersIterator = m_playerStates.find(userID);
        // .find() returns map.end() if the key is not found... should we do something special in
        // this class to denote if that happens?
        return m_getStatesOfAllPlayersIterator;
    }  

    std::map<int, std::vector<Environment>>::iterator InGameUserManager::getStatesOfAudience(int userID){
        std::map<int, std::vector<Environment>>::iterator m_getStatesOfAllAudiencesIterator;
        m_getStatesOfAllAudiencesIterator = m_audienceStates.find(userID);
        return m_getStatesOfAllAudiencesIterator;
    }
    
    std::map<int, std::vector<Environment>> InGameUserManager::getStatesOfAllPlayers(){
        return m_playerStates;
    }

    std::map<int, std::vector<Environment>> InGameUserManager::getStatesOfAllAudiences(){
        return m_audienceStates;
    }