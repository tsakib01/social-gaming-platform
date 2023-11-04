#ifndef IN_GAME_USER_MANAGER_H
#define IN_GAME_USER_MANAGER_H
#include <map> 
#include <string>
#include <vector>
#include <variant>
#include "GameStateLoader.h"

enum class Role {Player, Audience, Owner};

class InGameUserManager {

    public:
    InGameUserManager();

    // TODO: (GameInstance) Figure out when/how to call this class's functions.
    void addNewUser(uint32_t userID, Role role, Environment userStates);
    void deleteUser(uint32_t userID);
    Environment getStatesOfUser(uint32_t userID, Role role);
    // std::map<uint32_t, std::map<Role, Environment>> getAllUserStates();
    void setStatesOfUser(uint32_t userID, Role role, Environment states);

    private:
    ~InGameUserManager();
    std::map<uint32_t, std::pair<Role, Environment>> m_userStates;

};

#endif