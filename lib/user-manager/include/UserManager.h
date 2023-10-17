#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <algorithm>
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>

enum class Role : char {
    OWNER, PLAYER, AUDIENCE, NONE
};

struct User {
    uint32_t userID;
    std::string username;
    Role role;

    User(uint32_t userID, std::string username, Role role) : 
        userID{userID}, username{username}, role{role} {}; 
};

class UserManager {
public:
    UserManager() {};
    void addUser(uint32_t userID, std::string username);
    void setRole(uint32_t userID, Role role);
    void addToRoom(uint32_t userID, uint8_t roomCode);

private:
    std::vector<User> unassignedUsers;
    std::unordered_map<uint8_t, std::vector<User>> users;
};

#endif