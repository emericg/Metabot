#ifndef CTRL_H
#define CTRL_H

#include "metabot_config.h"

struct RobotStatus
{
    float speed = 1.0;
    float dx = 0.0;
    float dy = 0.0;
    float turn = 0.0;
    float height = 0.0;

    bool gait = GAIT_WALK;
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
    virtual void run(RobotStatus &status, bool &exit) = 0;
};
*/
#endif // CTRL_H
