
#include "ctrl_network.h"
#include "minitraces.h"

sf::Packet & operator <<(sf::Packet &packet, const MoveitMoveit &m)
{
    return packet << m.speed << m.dx << m.dy << m.turn << m.height << m.gait << m.crab << m.inverted;
}

sf::Packet & operator >>(sf::Packet &packet, MoveitMoveit &m)
{
    return packet >> m.speed >> m.dx >> m.dy >> m.turn >> m.height >> m.gait >> m.crab >> m.inverted;
}

networkControl::networkControl()
{
    //
}

networkControl::~networkControl()
{
    listener.close();
    client.disconnect();
}

bool networkControl::setup()
{
    bool status = false;

    listener.setBlocking(false);
    client.setBlocking(false);

    // Bind the listener to a given port
    if (listener.listen(port) == sf::Socket::Done)
    {
        TRACE_INFO(NET, "networkControl() Server listening on port '%i'", port);
        status = true;
    }
    else
    {
        TRACE_ERROR(NET, "networkControl() Error when binding listener to port '%i'!", port);
    }

    return status;
}

bool networkControl::autodetect()
{
    bool status = false;

    //if (client.getRemoteAddress() == sf::IpAddress::None)
    {
        // Accept a new connection?
        sf::Socket::Status com = listener.accept(client);
        if (com == sf::Socket::Done)
        {
            TRACE_INFO(NET, "Client connected: '%s @ %i'",
                       client.getRemoteAddress().toString().c_str(),
                       client.getRemotePort());
            status = true;
        }
        else if (com == sf::Socket::Error)
        {
            TRACE_WARNING(NET, "Socket error when trying to connect a client!");
        }
    }/*
    else
    {
        TRACE_WARNING(NET, "A client is already connected!");
    }*/

    return status;
}

bool networkControl::isConnected()
{
    bool status = false;

    // Do not account for brutal deconnections!
    if (client.getRemoteAddress() != sf::IpAddress::None)
    {
        status = true;
    }

    return status;
}

void networkControl::run(MoveitMoveit &move, bool &exit)
{
    bool status = false;

    // Check for new client(s)
    autodetect();

    if (isConnected() == true)
    {
        sf::Packet packet;
        sf::Socket::Status com = sf::Socket::NotReady;

        // Receiving packet(s), keep only the last unread packet
        do
        {
            com = client.receive(packet);

            if (com == sf::Socket::Done)
            {
                packet >> move;
                status = true;
            }
            else if (com == sf::Socket::Disconnected)
            {
                TRACE_INFO(NET, "Client disconnected: '%s @ %i'",
                           client.getRemoteAddress().toString().c_str(),
                           client.getRemotePort());
                client.disconnect();
            }
            else if (com == sf::Socket::Disconnected)
            {
                TRACE_WARNING(NET, "Socket error while receiving datas. WTF status: '%i'", (int)com);
            }
        }
        while (com == sf::Socket::Done);

        if (status == true)
        {
            TRACE_1(NET, "Received '%i' bytes [%f / %f / %f]", packet.getDataSize(), move.dx, move.dy, move.turn);
        }
    }
}
