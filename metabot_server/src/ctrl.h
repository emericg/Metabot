#ifndef CTRL_H
#define CTRL_H

#include "metabot_config.h"

struct MoveitMoveit
{
    float speed = 1;
    float dx = 0;
    float dy = 0;
    float turn = 0;
    float height = 0;

    bool gait = GAIT_DEFAULT;
    bool crab = false;
    bool inverted = false;
};
/*
class control
{
public:
    control();
    ~control();

    virtual bool setup() = 0;
    virtual bool autodetect() = 0;
    virtual bool isConnected() = 0;
    virtual void run(MoveitMoveit &move, bool &exit) = 0;
};
*/
#endif // CTRL_H
