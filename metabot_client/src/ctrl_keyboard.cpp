
#include "ctrl_keyboard.h"
#include "minitraces.h"

#include <SFML/Window.hpp>

keyboardControl::keyboardControl()
{
    //
}

keyboardControl::~keyboardControl()
{
    //
}

bool keyboardControl::autodetect()
{
    bool status = false;

    // Reset keyboard count
    keyboardCount = 0;

    // xinput list
    // grep -E 'Handlers|EV=' /proc/bus/input/devices | grep -B1 'EV=120013' | grep -Eo 'event[0-9]+'

    std::string cmd = "grep -E 'Handlers|EV=' /proc/bus/input/devices | grep -B1 'EV=120013' | grep -Eo 'event[0-9]+'";
    char buffer[128];
    std::string result = "";

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
    {
        return "ERROR";
    }

    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != nullptr)
        {
            result += buffer;
        }
    }
    pclose(pipe);

    if (result.empty() == false)
    {
        size_t pos = 0;
        while ((pos = result.find("\n", pos+1)) != std::string::npos)
        {
            TRACE_INFO(KEY, "Keyboard #%i is connected", keyboardCount);
            keyboardCount++;
            status = true;
        }
    }

    return status;
}

bool keyboardControl::isConnected()
{
    bool status = false;

    // Do not account for keyboard deconnections! But who would do that?
    if (keyboardCount > 0)
    {
        status = true;
    }

    return status;
}

void keyboardControl::run(MoveitMoveit &move, bool &exit)
{
    // Keyboard connected?
    if (isConnected() == false)
    {
        autodetect();
    }

    // Key
    ////////////////////////////////////////////////////////////////////////

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        exit = true;
        TRACE_INFO(KEY, "Escape key pressed. Exiting.");
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        TRACE_1(KEY, "W key pressed.");

        if (move.walk == false)
            move.walk = true;
        else
            move.walk = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
    {
        TRACE_1(KEY, "C key pressed.");

        if (move.crab == false)
            move.crab = true;
        else
            move.crab = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
    {
        TRACE_1(KEY, "I key pressed.");

        if (move.inverted == false)
            move.inverted = true;
        else
            move.inverted = false;
    }

    // Keypad
    ////////////////////////////////////////////////////////////////////////

    // Metabot height control
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
    {
        move.height = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
    {
        move.height = -1;
    }

    // Metabot translation control
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
    {
        move.dx = 100;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
    {
        move.dx = -100;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
    {
        move.dy = 100;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
    {
        move.dy = -100;
    }

    // Metabot rotation control
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9))
    {
        move.turn = 45;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7))
    {
        move.turn = -45;
    }

    // Recap
    ////////////////////////////////////////////////////////////////////////////

    if (move.dx || move.dy || move.turn)
    {
        TRACE_1(KEY, "DIRECTION (dx: %f) (dy: %f) (turn: %f)", move.dx, move.dy, move.turn);
    }
}
