#include "UserManager.h"

// std::vector<Player> UserManager::getPlayers(){
//     return m_players;
// }

// std::vector<Spectator> UserManager::getSpectators(){
//     return m_spectators;
// }

//Initial setup of per_player map.
void UserManager::setPlayers(std::vector<Player> playerList){
    for(Player player : playerList){
        m_playerStates[player.getName()];
    }
}

// Update whole set of player's assignment values; is there a way for us to just update
// a smaller, select number of values instead? Maybe here or elsewhere?
void UserManager::setPlayerValues(Player player, std::vector<Value> values){
    m_playerStates[player.getName()] = values;
}

std::map<std::string, std::vector<Value>> UserManager::getAllPlayerStates(){
    return m_playerStates;
}

// Initial setup of maps for spectators
void UserManager::setSpectators(std::vector<Spectator> spectatorList){
    for(Spectator spectator : spectatorList){
        m_spectatorStates[spectator.getUserID()];
    }
}

void UserManager::setSpectatorValues(Spectator spectator, std::vector<Value> value){ 
    m_spectatorStates[spectator.getUserID()] = value;
}

std::map<int, std::vector<Value>> UserManager::getAllSpectatorStates(){
    return m_spectatorStates;
}

// TODO: Figure out how/where we can get a specific player or spectator.