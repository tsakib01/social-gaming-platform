#ifndef SPECTATOR_H
#define SPECTATOR_H

// Unlike a Player, a Spectator can be a host.
class Spectator{
    public:
    Spectator(int userID, bool isHost){
        userID = userID;
        isHost = isHost;
    }
    int getUserID();
    bool isHost();
    ~Spectator();
    
    private:
    int userID;
    bool isAHost;
};

#endif