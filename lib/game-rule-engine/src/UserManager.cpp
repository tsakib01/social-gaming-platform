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

// Update whole set of player's assignment values; is there a way for us to just update
// a smaller, select number of values instead? Maybe here or elsewhere?
void UserManager::setPlayerValues(Player player, std::vector<Value> values){
    per_player[player.getName()] = values;
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

void UserManager::setSpectatorValue(Spectator spectator, std::vector<Value> value){ 
    per_spectator[spectator.getUserID()] = value;
}
//TODO: create functions that will update the maps from setPlayers and setSpectators