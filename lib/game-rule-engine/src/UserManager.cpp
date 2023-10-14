#include "UserManager.h"

std::vector<Player> UserManager::getPlayers(){
    return players;
}

//Initial setup of per_player map.
void UserManager::setPlayers(std::vector<Player> playerList){
    for(Player player : playerList){
        per_player[player.getName()];
    }
}

std::vector<Spectator> UserManager::getSpectators(){
    return spectators;
}

// Initial setup of maps for spectators
void UserManager::setSpectators(std::vector<Spectator> spectatorList){
    for(Spectator spectator : spectatorList){
        per_spectator[spectator.getUserID()];
    }
}

//TODO: create functions that will update the maps from setPlayers and setSpectators