#ifndef NETWORK_H
#define NETWORK_H

#include "ctrl.h"
#include <SFML/Network.hpp>

class networkClient
{
    unsigned port = 5555;
    sf::TcpSocket socket;

    MoveitMoveit last;

public:
    networkClient();
    ~networkClient();

    bool autodetect();
    bool isConnected();

    bool send(MoveitMoveit &move);
    void tick();
};

#endif // NETWORK_H
