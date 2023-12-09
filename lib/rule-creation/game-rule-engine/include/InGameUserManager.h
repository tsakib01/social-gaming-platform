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
    std::map<uintptr_t, GameEnvironment::Environment> getAllUserStates();
    std::vector<UserId> getAllUserIds() const;
    //const std::unique_ptr<GameEnvironment::Value> getValueOfUser(UserId userID, GameEnvironment::Identifier identifier);
    GameEnvironment::Value getValueOfUser(UserId userID, GameEnvironment::Identifier identifier);
    void setIdentifierOfUser(UserId userID, GameEnvironment::Identifier identifier, std::unique_ptr<GameEnvironment::Value> value);
    
    private:
    // Store all states in a single Value
    std::map<uintptr_t, GameEnvironment::Environment> m_userStates;

};

#endif