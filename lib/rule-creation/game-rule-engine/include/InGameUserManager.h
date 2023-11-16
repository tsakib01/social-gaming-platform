#ifndef IN_GAME_USER_MANAGER_H
#define IN_GAME_USER_MANAGER_H

#include "GameEnvironment.h"
#include "UserManager.h"
#include <map> 
#include <string>
#include <vector>
#include <variant>

class InGameUserManager { 

    public:
    InGameUserManager(){};
    ~InGameUserManager(){};

    // TODO: (GameInstance) Figure out when/how to call this class's functions.
    void addNewUser(networking::Connection userID, Role role, GameEnvironment::Environment userStates);
    void deleteUser(networking::Connection userID);
    GameEnvironment::Environment getStatesOfUser(networking::Connection userID);
    std::map<intptr_t, std::pair<Role, GameEnvironment::Environment>> getAllUserStates();
    void setStatesOfUser(networking::Connection  userID, Role role, GameEnvironment::Environment states);

    private:
    std::map<intptr_t, std::pair<Role, GameEnvironment::Environment>> m_userStates;

};

#endif