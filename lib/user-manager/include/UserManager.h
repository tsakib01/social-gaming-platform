#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <algorithm>
#include <vector>
#include <string_view>
#include <cstdint>
#include "Server.h"

using networking::Connection;

enum class Role : char {
    OWNER, 
    PLAYER, 
    AUDIENCE, 
    NONE
};

enum class UserState : char {
    NEW, 
    INTRO, 
    JOIN_GAME, 
    GAME_SELECT, 
    GAME_CONFIG,
    GAME_WAIT,
    GAME_RUN,
    NONE
};


struct User {
    Connection userID;
    std::string username;
    Role role;
    UserState state;
    uint16_t roomCode;

    User(Connection userID) : userID{userID}, state{UserState::NEW} {};
};

class UserManager {
public:
    UserManager() {};
    void addUser(Connection userID);
    void removeUser(Connection userID);

    void setUserName(Connection userID, std::string username);
    void setUserRole(Connection userID, Role role);
    void setUserRoomCode(Connection userID, uint16_t roomCode);
    void setUserState(Connection userID, UserState state);

    std::vector<User> getUsersInGame(Connection userID);
    uint16_t getUserGameCode(Connection userID);
    std::vector<User> getAllUsers() const { return users; }
    std::vector<User>::iterator findUserByID(Connection userID);
    std::vector<User>::iterator getRoomOwner(uint16_t roomCode);

private:
    std::vector<User> users;
};

#endif