#ifndef CTRL_KEYBOARD_H
#define CTRL_KEYBOARD_H

#include "ctrl.h"

class keyboardControl
{
    int keyboardCount;

public:
    keyboardControl();
    ~keyboardControl();

    bool autodetect();
    bool isConnected();
    void run(RobotStatus &rs, bool &exit);
};


#endif // CTRL_KEYBOARD_H
