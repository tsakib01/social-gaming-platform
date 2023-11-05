#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <algorithm>
#include <vector>
#include <string_view>
#include <cstdint>
#include "Server.h"

using networking::Connection;

enum class Role : char {
    OWNER, PLAYER, AUDIENCE, NONE
};

enum class UserState : char {
    NEW, JOIN, CREATE, CHOOSE_GAME, SET_CONFIG, START_GAME, GAME_INPUT, GAME_INACTIVE
};

struct User {
    Connection userID;
    std::string_view username;
    Role role;
    UserState state;
    uint8_t roomCode;

    User(Connection userID) : userID{userID}, state{UserState::NEW} {};
};

class UserManager {
public:
    UserManager() {};
    void addUser(Connection userID);
    void setUserName(Connection userID, std::string_view username);
    void setUserRole(Connection userID, Role role);
    void setUserRoomCode(Connection userID, uint8_t roomCode);
    void setUserState(Connection userID, UserState state);
    void removeUser(Connection userID);
    std::vector<User> getUsersInGame(Connection userID);
    uint8_t getUserGameCode(Connection userID);
    std::vector<User> getAllUsers() const { return users; }
    std::vector<User>::iterator findUserByID(Connection userID);

private:
    std::vector<User> users;
};

#endif