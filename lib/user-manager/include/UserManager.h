#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <algorithm>
#include <vector>
#include <string_view>
#include <cstdint>

enum class Role : char {
    OWNER, PLAYER, AUDIENCE, NONE
};

struct User {
    uint32_t userID;
    std::string_view username;
    Role role;
    uint8_t roomCode;

    User(uint32_t userID, std::string_view username, Role role, uint8_t roomCode) : 
        userID{userID}, username{username}, role{role}, roomCode{roomCode} {}; 
};

class UserManager {
public:
    UserManager() {};
    void addUser(uint32_t userID, std::string_view username);
    void setRole(uint32_t userID, Role role);
    void setRoom(uint32_t userID, uint8_t roomCode);
    void removeUser(uint32_t userID);

private:
    std::vector<User> users;
};

#endif