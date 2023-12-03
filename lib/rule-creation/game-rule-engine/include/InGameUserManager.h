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

    // Called in GameInstance
    void addNewUser(UserId connection, GameEnvironment::Environment userStates);
    void deleteUser(UserId connection);
    
    // TODO: (GameInstance) Figure out when/how to call this class's functions.
    GameEnvironment::Environment& getStatesOfUser(UserId userID);
    std::map<uintptr_t, GameEnvironment::Environment> getAllUserStates();

    const std::unique_ptr<GameEnvironment::Value> getValueOfUser(UserId userID, GameEnvironment::Identifier identifier);

    void setStatesOfUser(UserId userID, GameEnvironment::Environment states);
    void setIdentifierOfUser(UserId userID, GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value> value);
    private:
    std::map<uintptr_t, GameEnvironment::Environment> m_userStates;

};

#endif