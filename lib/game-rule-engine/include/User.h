#ifndef USER_H
#define USER_H
// Can denote a Player or a Spectator.
#include <string>
class User {
    public:
    std::string getName();
    int getUserID();

    private:
    std::string name;
    int userID;
};

#endif