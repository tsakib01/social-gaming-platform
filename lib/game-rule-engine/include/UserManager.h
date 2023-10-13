#ifndef USER_MANAGER_H
#define USER_MANAGER_H
#include "Player.h"
#include "Spectator.h"
#include <unordered_map> 
#include <string>
#include <vector>
class UserManager {
    public:

    private:
    UserManager(std::vector<Player> players, std::vector<Spectator> spectators);
    // Use an unordered_map to distinguish ebtween players and audiences
    std::unordered_map<std::string, int>per_player; 
    std::unordered_map<std::string, int>per_audience;
};

#endif