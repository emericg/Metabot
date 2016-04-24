
#include "network.h"
#include "minitraces.h"

sf::Packet & operator <<(sf::Packet &packet, const MoveitMoveit &m)
{
    return packet << m.speed << m.dx << m.dy << m.turn << m.height << m.walk << m.crab << m.inverted;
}

sf::Packet & operator >>(sf::Packet &packet, MoveitMoveit &m)
{
    return packet >> m.speed >> m.dx >> m.dy >> m.turn >> m.height >> m.walk >> m.crab >> m.inverted;
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
        TRACE_INFO(COM, "Connection to server '%s @ %i' successfull!\n", ip.toString().c_str(), port);
    }
    else
    {
        TRACE_ERROR(COM, "Connection to server '%s @ %i' failed!\n", ip.toString().c_str(), port);
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

bool networkClient::send(MoveitMoveit &move)
{
    bool status = false;

    if (move.dx == 0 && move.dy == 0 && move.turn == 0 && move.height == 0)
    {
        if (move.walk == last.walk && move.crab == last.crab && move.inverted == last.inverted)
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
        TRACE_ERROR(COM, "Unable to send data packet!\n");
    }

    return status;
}

void networkClient::tick()
{
    //
}