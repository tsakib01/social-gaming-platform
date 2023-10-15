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
        players = players;
        spectators = spectators;
    }

    std::vector<Player> getPlayers();
    std::vector<Spectator> getSpectators();
    
    void setPlayers(std::vector<Player> players);
    void setSpectators(std::vector<Spectator> spectators);
    void setPlayerValues(Player player, std::vector<Value>values);
    void setSpectatorValue(Spectator spectator, std::vector<Value> values);

    //TODO: Figure out how to call getters and setters

    private:
    ~UserManager();

    std::vector<Player> players;
    std::map<std::string, std::vector<Value>> per_player; 
    std::vector<Spectator> spectators;
    // Spectators don't have unique names, so we can just use their user IDs which are of type int
    std::map<int, std::vector<Value>>per_spectator;
};

#endif