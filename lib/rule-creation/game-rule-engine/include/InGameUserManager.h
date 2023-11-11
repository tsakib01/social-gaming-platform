#ifndef IN_GAME_USER_MANAGER_H
#define IN_GAME_USER_MANAGER_H

//#include "GameStateLoader.h"
#include "GameEnvironment.h"
#include <map> 
#include <string>
#include <vector>
#include <variant>
#include <GameEnvironment.h>

enum class Role {PLAYER, AUDIENCE, OWNER};

class InGameUserManager { 

    public:
    InGameUserManager(){};
    ~InGameUserManager(){};

    // TODO: (GameInstance) Figure out when/how to call this class's functions.
    void addNewUser(uint32_t userID, Role role, GameEnvironment::Environment userStates);
    void deleteUser(uint32_t userID);
    GameEnvironment::Environment getStatesOfUser(uint32_t userID);
    std::map<uint32_t, std::pair<Role, GameEnvironment::Environment>> getAllUserStates();
    void setStatesOfUser(uint32_t userID, Role role, GameEnvironment::Environment states);

    private:
    std::map<uint32_t, std::pair<Role, GameEnvironment::Environment>> m_userStates;

};

#endif