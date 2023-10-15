#ifndef SPECTATOR_H
#define SPECTATOR_H

// Unlike a Player, a Spectator can be a host.
class Spectator{
    public:
    Spectator(int userID, bool isHost){
        m_userID = userID;
        m_isHost = isHost;
    }
    int getUserID();
    bool isHost();
    ~Spectator();
    
    private:
    int m_userID;
    bool m_isHost;
};

#endif