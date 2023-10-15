#ifndef USER_MANAGER_H
#define USER_MANAGER_H
#include "Player.h"
#include "Spectator.h"
#include <map> 
#include <string>
#include <vector>
#include <variant>
using Primitive = std::variant<int, bool, std::string_view>;
using Map = std::map<std::string_view, Primitive>;
using List = std::vector<std::variant<Primitive, Map>>;
using Value = std::variant<Primitive, Map, List>;

class UserManager {
    public:
    UserManager(std::vector<Player> players, std::vector<Spectator> spectators){
        // Having member variables holding the players/spectators may be 
        // redundant if we already have maps...
        // m_players = players;
        // m_spectators = spectators;

        // Have to call function to instantiate empty map with keys but no values
        setPlayers(players);
        setSpectators(spectators);

    }


    //TODO: Figure out when/howto call getters/setters
    std::vector<Player> getPlayers();
    std::vector<Spectator> getSpectators();

    void setPlayers(std::vector<Player> players);
    void setSpectators(std::vector<Spectator> spectators);
    void setPlayerValues(Player player, std::vector<Value>values);
    void setSpectatorValues(Spectator spectator, std::vector<Value> values);
    std::map<std::string, std::vector<Value>> getAllPlayerStates();
    std::map<int, std::vector<Value>> getAllSpectatorStates();

    private:
    ~UserManager();

    // std::vector<Player> m_players;
    // std::vector<Spectator> m_spectators;

    // Spectators don't have unique names, so we can just use their user IDs which are of type int
    std::map<std::string, std::vector<Value>> m_playerStates; 
    std::map<int, std::vector<Value>>m_spectatorStates;
};

#endif