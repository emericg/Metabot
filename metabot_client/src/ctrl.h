#ifndef CTRL_H
#define CTRL_H

struct RobotStatus
{
    float speed = 1;
    float dx = 0;
    float dy = 0;
    float turn = 0;
    float height = 0;

    bool walk = false;
    bool crab = false;
    bool inverted = false;
};
/*
class control
{
public:
    control();
    ~control();

    virtual bool autodetect() = 0;
    virtual void tick(MoveitMoveit &move) = 0;
};
*/
#endif // CTRL_H
