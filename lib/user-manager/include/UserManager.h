#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <algorithm>
#include <vector>
#include <string_view>
#include <cstdint>
#include "Server.h"

using UserId = networking::Connection;

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
    UserId userID;
    std::string username;
    Role role;
    UserState state;
    uint16_t roomCode;

    User(UserId userID) : userID{userID}, state{UserState::NEW} {};
};

class UserManager {
public:
    UserManager() {};

    void addUser(UserId userID);
    void removeUser(UserId userID);

    void setUserName(UserId userID, std::string_view username);
    void setUserRole(UserId userID, Role role);
    void setUserRoomCode(UserId userID, uint16_t roomCode);
    void setUserState(UserId userID, UserState state);

    std::vector<User> getAllUsers() const;
    std::vector<User> getUsersInGame(uint16_t roomCode) const;
    User getUserByID(UserId userID) const;
    User getRoomOwner(uint16_t roomCode) const;

private:
    std::vector<User> m_users;
    User& getUserReferenceByID(UserId userID);
};

#endif