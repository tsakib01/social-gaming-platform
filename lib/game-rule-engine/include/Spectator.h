#ifndef SPECTATOR_H
#define SPECTATOR_H
#include "User.h"

class Spectator : public User{
    public:
    bool isHost;
    private:
};

#endif