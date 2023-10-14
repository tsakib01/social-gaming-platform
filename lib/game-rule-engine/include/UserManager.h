#ifndef USER_MANAGER_H
#define USER_MANAGER_H
#include "Player.h"
#include "Spectator.h"
#include <unordered_map> 
#include <string>
#include <vector>
#include <variant>

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

    //TODO: Figure out how to call getters and setters

    private:
    ~UserManager();
    std::vector<Player> players;
    // Keys for for players TENTATIVELY maps from their name to a variant that supports
    // an "empty" variant (aka monostate), int, bool and string.
    std::unordered_map<std::string, std::vector<std::variant<std::monostate, int, bool, std::string>>> per_player; 
    std::vector<Spectator> spectators;
    // Spectators don't have unique names, so we can just use their user IDs which are of type int
    std::unordered_map<int, std::vector<std::variant<std::monostate, int, bool, std::string>>>per_spectator;
};

#endif