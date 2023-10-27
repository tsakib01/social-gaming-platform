#ifndef IN_GAME_USER_MANAGER_H
#define IN_GAME_USER_MANAGER_H
#include <map> 
#include <string>
#include <vector>
#include <variant>
#include "GameStateLoader.h"

enum Role {Player, Audience, Owner};

using Identifier = std::string_view;
using Primitive = std::variant<int, bool, std::string_view>;
using Map = std::map<std::string_view, Primitive>;
using List = std::vector<std::variant<Primitive, Map>>;
using Value = std::variant<Primitive, Map, List>;
using Environment = std::map<Identifier, Value>;

using InGameState = std::map<uint32_t, std::map<Role, Environment>>;
//The actual map that holds the user id, plus their role and state
//map id -> to map of enum, Environment
// Keep track of all users in a particular game.
// Currently, not specifically distinguishing the Owner of a game from other Spectators.
class InGameUserManager {

    public:
    InGameUserManager();

    // TODO: (GameInstance) Figure out when/how to call this class's functions.
    void addNewUser(uint32_t userID, Role role, Environment userStates);

    // Do we want to keep track of states belonging to a Player that has left a game?
    void deleteUser(uint32_t userID);

    // Get a specific user's states from their corresponding map.
    Environment getStatesOfUser(uint32_t userID, Role role);
    void setStatesOfUser(uint32_t userID, Role role, Environment states);

    private:
    ~InGameUserManager();
    std::map<uint32_t, std::map<Role, Environment>> m_userStates;
};

#endif