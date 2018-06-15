#ifndef NETWORK_H
#define NETWORK_H

#include "../metabot_server/src/ctrl.h"

#include <SFML/Network.hpp>

class networkClient
{
    unsigned port = 5555;
    sf::TcpSocket socket;

    RobotStatus last;

public:
    networkClient();
    ~networkClient();

    bool autodetect();
    bool isConnected();

    bool send(RobotStatus &move);
    void tick();
};

#endif // NETWORK_H
