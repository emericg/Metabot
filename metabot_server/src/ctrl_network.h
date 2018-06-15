#ifndef CTRL_NETWORK_H
#define CTRL_NETWORK_H

#include "ctrl.h"
#include <SFML/Network.hpp>

class networkControl
{
    unsigned port = 5555;

    sf::TcpListener listener;
    sf::TcpSocket client;

public:
    networkControl();
    ~networkControl();

    bool setup();
    bool autodetect();
    bool isConnected();
    void run(RobotStatus &rs, bool &exit);
};

#endif // CTRL_NETWORK_H
