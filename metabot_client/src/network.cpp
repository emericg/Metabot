#include "network.h"

#include "../metabot_server/src/minitraces.h"

sf::Packet & operator <<(sf::Packet &packet, const RobotStatus &m)
{
    return packet << m.speed << m.dx << m.dy << m.turn << m.height << m.gait << m.crab << m.inverted;
}

sf::Packet & operator >>(sf::Packet &packet, RobotStatus &m)
{
    return packet >> m.speed >> m.dx >> m.dy >> m.turn >> m.height >> m.gait >> m.crab >> m.inverted;
}

networkClient::networkClient()
{
    //
}

networkClient::~networkClient()
{
    socket.disconnect();
}

bool networkClient::autodetect()
{
    bool status = false;

    sf::IpAddress ip("metabot.local");

    // ping?

    // connection
    sf::Socket::Status com = socket.connect(ip, port);
    if (com == sf::Socket::Done)
    {
        status = true;
        TRACE_INFO(NET, "Connection to server '%s @ %i' successfull!", ip.toString().c_str(), port);
    }
    else
    {
        TRACE_ERROR(NET, "Connection to server '%s @ %i' failed!", ip.toString().c_str(), port);
    }

    return status;
}

bool networkClient::isConnected()
{
    bool status = false;

    if (socket.getRemoteAddress() != sf::IpAddress::None)
    {
        status = true;
    }

    return status;
}

bool networkClient::send(RobotStatus &move)
{
    bool status = false;

    if (move.dx == 0 && move.dy == 0 && move.turn == 0 && move.height == 0)
    {
        if (move.gait == last.gait && move.crab == last.crab && move.inverted == last.inverted)
        {
            return status;
        }
    }

    if (isConnected() == false)
    {
        if (autodetect() == false)
        {
            return status;
        }
    }

    // Form packet
    sf::Packet packet;
    packet << move;

    // Send packet
    if (socket.send(packet) == sf::Socket::Done)
    {
        status = true;
        last = move;
    }
    else
    {
        TRACE_ERROR(NET, "Unable to send data packet!");
    }

    return status;
}

void networkClient::tick()
{
    //
}
