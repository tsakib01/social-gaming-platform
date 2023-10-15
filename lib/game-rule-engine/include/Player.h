#ifndef PLAYER_H
#define PLAYER_H
#include <string>

//Unlike a Spectator, a Player has a unique name in addition to their ID.
class Player{
    public:
    Player(int userID, std::string playerName){
        m_userID = userID;
        m_playerName = playerName;
    }

    ~Player();
    int getUserID();
    std::string getName();

    private:
    int m_userID;
    std::string m_playerName;
};

#endif