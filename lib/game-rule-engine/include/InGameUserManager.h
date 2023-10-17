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
    // per-audience-states go to addNewSpectator.
    // GameInstance should somehow select the right one to use.

    void addNewPlayer(int userID, std::vector<Environment> playerStates); 
    void addNewAudience(int userID, std::vector<Environment> audienceStates);

    // Do we want to keep track of states belonging to a Player that has left a game?
    void deletePlayer(int userID);
    void deleteAudience(int UserID);

    // Get a specific user's states from their corresponding map.
    std::map<int, std::vector<Environment>>::iterator getStatesOfPlayer(int userID);
    std::map<int, std::vector<Environment>>::iterator getStatesOfAudience(int userID);
    
    std::map<int, std::vector<Environment>> getStatesOfAllPlayers();
    std::map<int, std::vector<Environment>> getStatesOfAllAudiences();

    private:
    ~InGameUserManager();
    
    std::map<int, std::vector<Environment>> m_playerStates;
    std::map<int, std::vector<Environment>> m_audienceStates;

    std::vector<Environment>::iterator m_getStatesOfAllPlayersIterator;
    std::vector<Environment>::iterator m_getStatesOfAllAudiencesIterator;
};

#endif