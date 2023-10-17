#ifndef IN_GAME_USER_MANAGER_H
#define IN_GAME_USER_MANAGER_H
#include "Player.h"
#include "Spectator.h"
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

    void addNewPlayer(int userID, Environment playerStates); 
    void addNewAudience(int userID, Environment audienceStates);

    // Do we want to keep track of states belonging to a Player that has left a game?
    void deletePlayer(int userID);
    void deleteAudience(int userID);

    // Get a specific user's states from their corresponding map.
    std::map<int, Environment>::iterator getStatesOfPlayer(int userID);
    std::map<int, Environment>::iterator getStatesOfAudience(int userID);
    
    std::map<int, Environment> getStatesOfAllPlayers();
    std::map<int, Environment> getStatesOfAllAudiences();

    private:
    ~InGameUserManager();
    
    std::map<int, Environment> m_playerStates;
    std::map<int, Environment> m_audienceStates;

    Environment::iterator m_getStatesOfAllPlayersIterator;
    Environment::iterator m_getStatesOfAllAudiencesIterator;
};

#endif