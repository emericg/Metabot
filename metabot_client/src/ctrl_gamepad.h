#ifndef CTRL_GAMEPAD_H
#define CTRL_GAMEPAD_H

#include "ctrl.h"

class gamepadControl
{
    int joystickId = -1;
    int joystickAxis = -1;
    int joystickButtons = -1;

public:
    gamepadControl();
    ~gamepadControl();

    bool autodetect();
    bool isConnected();
    void run(RobotStatus &move, bool &exit);
};

#endif // CTRL_GAMEPAD_H
