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
    void run(MoveitMoveit &move, bool &exit);
};


#endif // CTRL_KEYBOARD_H
