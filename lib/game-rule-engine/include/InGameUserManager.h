#ifndef IN_GAME_USER_MANAGER_H
#define IN_GAME_USER_MANAGER_H
#include <map> 
#include <string>
#include <vector>
#include <variant>
#include "GameStateLoader.h"

// Keep track of all users in a particular game.
// Currently, not specifically distinguishing the Owner of a game from other Spectators.
class InGameUserManager {

    public:
    InGameUserManager();

    // TODO: (GameInstance) Figure out when/how to call this class's functions.
    // We will assume that per-player states go to addNewPlayer, and 
    // per-audience-states go to addNewAudience.
    // GameInstance should somehow select the right one to use.

    void addNewPlayer(uint32_t userID, Environment playerStates); 
    void addNewAudience(uint32_t userID, Environment audienceStates);

    // Do we want to keep track of states belonging to a Player that has left a game?
    void deleteUser(uint32_t userID);

    // Get a specific user's states from their corresponding map.
    Environment getStatesOfUser(uint32_t userID);
    std::map<uint32_t, Environment> getStatesOfAllPlayers();
    std::map<uint32_t, Environment> getStatesOfAllAudiences();

    void setStatesOfUser(uint32_t userID, Environment states);

    private:
    ~InGameUserManager();
    
    std::map<uint32_t, Environment> m_playerStates;
    std::map<uint32_t, Environment> m_audienceStates;

};

#endif