
#include "ctrl_gamepad.h"
#include "minitraces.h"

#include <cmath>

#include <SFML/Window.hpp>

/*
    // Xbox 360 gamepad mapping:
    // 0 is A
    // 1 is B
    // 2 is X
    // 3 is Y
    // 4 is LB
    // 5 is RB
    // 6 is SELECT
    // 7 is START
    // 8 is "xbox"
    // 9 is left joy click
    // 10 is right joy click
*/

gamepadControl::gamepadControl()
{
    //
}

gamepadControl::~gamepadControl()
{
    //
}

bool gamepadControl::autodetect()
{
    bool status = false;

    // Poll events, must be done in order for gamepads to be detected
    sf::Joystick::update();

    // Reset current settings
    joystickId = -1;
    joystickAxis = -1;
    joystickButtons = -1;

    // Look for gamepads, SFML can handle up to 8 but we only need the first one
    for (int i = 0; i < sf::Joystick::Count; i++)
    {
        if (sf::Joystick::isConnected(i))
        {
            joystickId = i;
            joystickAxis = 0;
            joystickButtons = sf::Joystick::getButtonCount(joystickId);

            // Count axis manually
            for (int j = 0; j < sf::Joystick::AxisCount; j++)
            {
                if (sf::Joystick::hasAxis(joystickId, (sf::Joystick::Axis)(j)) == true)
                {
                    joystickAxis++;
                }
            }

            TRACE_INFO(PAD, "Joystick #%i is connected with %i axis and %i buttons.", joystickId, joystickAxis, joystickButtons);
            status = true;
            break;
        }
    }

    return status;
}

bool gamepadControl::isConnected()
{
    bool status = false;

    if (joystickId >= 0 && sf::Joystick::isConnected(joystickId) == true)
    {
        status = true;
    }

    return status;
}

void gamepadControl::run(RobotStatus &rs, bool &exit)
{
    // Poll events
    sf::Joystick::update();

    // Gamepad still connected?
    if (isConnected() == false)
    {
        autodetect();
    }

    // Gamepad connected?
    if (isConnected() == true)
    {
        // Buttons
        ////////////////////////////////////////////////////////////////////////

        if (sf::Joystick::isButtonPressed(joystickId, 6))
        {
            exit = true;
            TRACE_INFO(PAD, "SELECT button pressed. Exiting.");
        }
        if (sf::Joystick::isButtonPressed(joystickId, 0))
        {
            TRACE_1(PAD, "A button pressed.");

            if (rs.gait == false)
                rs.gait = true;
            else
                rs.gait = false;
        }
        if (sf::Joystick::isButtonPressed(joystickId, 1))
        {
            TRACE_1(PAD, "B button pressed.");

            if (rs.crab == false)
                rs.crab = true;
            else
                rs.crab = false;
        }
        if (sf::Joystick::isButtonPressed(joystickId, 3))
        {
            TRACE_1(PAD, "Y button pressed.");

            if (rs.inverted == false)
                rs.inverted = true;
            else
                rs.inverted = false;
        }

        // Triggers
        ////////////////////////////////////////////////////////////////////////

        //float lt = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::Z); // left trigger
        //float rt = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::R); // right trigger

        // Pad
        ////////////////////////////////////////////////////////////////////////

        //float pad_x = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::PovX); // keypad, horizontal
        float pad_y = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::PovY); // keypad, vertical

        // Metabot height control
        if (pad_y == -100)
        {
            rs.height = 1;
        }
        else if (pad_y == 100)
        {
            rs.height = -1;
        }

        // Sticks
        ////////////////////////////////////////////////////////////////////////

        float x = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::X); // left stick, horizontal
        float y = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::Y); // left stick, vertical
        float u = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::U); // right stick, horizontal
        float v = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::V); // right stick, vertical

        // Metabot translation control
        if (u > 50.0 || u < -50.0) // dead zone ?
        {
            rs.dy = u;
        }
        if (v > 50.0 || v < -50.0) // dead zone ?
        {
            rs.dx = -v;
        }

        // Metabot direction control
        if ((y > 24.0 || y < -24.0) || (x > 24.0 || x < -24.0)) // dead zone ?
        {
            // Compute stick angle
            float ang = std::atan2(x, y) * 57.2957795 + 180.0;

            if (ang >= 270)
                rs.turn = -(ang - 360);
            else if (ang >= 180)
                rs.turn = ang - 180;
            else if (ang >= 90)
                rs.turn = ang - 180;
            else
                rs.turn = -ang;

            // forward/backward dead zone ?
            if (rs.turn < 28.0 && rs.turn > -28.0)
                rs.turn = 0;

            rs.dx = -y;

            // turn zone
            if (ang < 110.0 && ang > 70.0)
            {
                rs.turn = -60; // rotate to the left
                rs.dx = 0;
            }
            if (ang < 290.0 && ang > 250.0)
            {
                rs.turn = 60; // rotate to the right
                rs.dx = 0;
            }
        }

        // Recap
        ////////////////////////////////////////////////////////////////////////

        if (rs.dx || rs.dy || rs.turn)
        {
            TRACE_1(PAD, "DIRECTION (dx: %f) (dy: %f) (turn: %f)", rs.dx, rs.dy, rs.turn);
        }
    }
}
