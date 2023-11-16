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
    void addNewUser(intptr_t userID, Role role, GameEnvironment::Environment userStates);
    void deleteUser(intptr_t  userID);
    GameEnvironment::Environment getStatesOfUser(intptr_t userID);
    std::map<intptr_t , std::pair<Role, GameEnvironment::Environment>> getAllUserStates();
    void setStatesOfUser(intptr_t  userID, Role role, GameEnvironment::Environment states);

    private:
    std::map<intptr_t, std::pair<Role, GameEnvironment::Environment>> m_userStates;

};

#endif