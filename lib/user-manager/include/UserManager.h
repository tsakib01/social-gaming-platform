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

struct User {
    Connection userID;
    std::string_view username;
    Role role;
    uint16_t roomCode;

    User(Connection userID) : userID{userID} {};
};

class UserManager {
public:
    UserManager() {};
    bool addUser(Connection userID);
    bool setUserName(Connection userID, std::string_view username);
    bool setUserRole(Connection userID, Role role);
    bool setUserRoomCode(Connection userID, uint16_t roomCode);
    bool removeUser(Connection userID);
    std::vector<User> getUsersInGame(Connection userID);
    uint16_t getUserRoomCode(Connection userID);

private:
    std::vector<User> users;
    std::vector<User>::iterator findUserByID(Connection userID);
};

#endif