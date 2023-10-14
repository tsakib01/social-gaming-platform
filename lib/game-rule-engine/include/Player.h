#ifndef PLAYER_H
#define PLAYER_H
#include <string>

//Unlike a Spectator, a Player has a unique name in addition to their ID.
class Player{
    public:
    Player(int userID, std::string playerName){
        userID = userID;
        playerName = playerName;
    }

    ~Player();
    int getUserID();
    std::string getName();

    private:
    int userID;
    std::string playerName;
};

#endif