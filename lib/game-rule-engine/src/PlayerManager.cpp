#include "PlayerManager.h"

// std::vector<Player> UserManager::getPlayers(){
//     return m_players;
// }

// std::vector<Spectator> UserManager::getSpectators(){
//     return m_spectators;
// }

//Initial setup of per_player map.
void PlayerManager::setPlayers(std::vector<Player> playerList){
    for(Player player : playerList){
        m_playerStates[player.getName()];
    }
}

// Update whole set of player's assignment values; is there a way for us to just update
// a smaller, select number of values instead? Maybe here or elsewhere?
void PlayerManager::setPlayerValues(Player player, std::vector<Value> values){
    m_playerStates[player.getName()] = values;
}

std::map<std::string, std::vector<Value>> PlayerManager::getAllPlayerStates(){
    return m_playerStates;
}

// Initial setup of maps for spectators
void PlayerManager::setSpectators(std::vector<Spectator> spectatorList){
    for(Spectator spectator : spectatorList){
        m_spectatorStates[spectator.getUserID()];
    }
}

void PlayerManager::setSpectatorValues(Spectator spectator, std::vector<Value> value){ 
    m_spectatorStates[spectator.getUserID()] = value;
}

std::map<int, std::vector<Value>> PlayerManager::getAllSpectatorStates(){
    return m_spectatorStates;
}

// TODO: Figure out how/where we can get a specific player or spectator.