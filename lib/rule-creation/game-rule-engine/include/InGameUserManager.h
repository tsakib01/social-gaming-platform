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
    InGameUserManager(){
        // uintptr_t testID = 1;
        // GameEnvironment::Environment testState
        // m_userStates[testID] = testState; 
    };
    ~InGameUserManager(){};

    // TODO: (GameInstance) Figure out when/how to call this class's functions.
    void addNewUser(networking::Connection connection, GameEnvironment::Environment userStates);
    void deleteUser(networking::Connection connection);
    GameEnvironment::Environment getStatesOfUser(networking::Connection userID);
    std::map<uintptr_t, GameEnvironment::Environment> getAllUserStates();
    void setStatesOfUser(networking::Connection userID, GameEnvironment::Environment states);

    private:
    std::map<uintptr_t, GameEnvironment::Environment> m_userStates;

};

#endif