/*!
 * The Metabot project is under CC BY-NC license: http://creativecommons.org/licenses/by-nc/3.0/
 * The authors are:
 * - Robot Campus
 * - Rhoban System S.A.S
 */

#ifndef METABOT_CONTROL_H
#define METABOT_CONTROL_H

// SmartServoFramework
#include <SmartServoFramework/SimpleAPI.h>

#include "ctrl.h"
#include "metabot_function.h"
#include "metabot_config.h"

class Metabot
{
    //! Servos communication API
    DynamixelSimpleAPI *dxl = nullptr;

    int angle_to_step(float angle);
    float step_to_angle(int step);

    /*!
     * This is the servos configuration, you can change the zero
     * or the min/max limits here.
     */
    void setupLimits();
    void setupSettings();
    void setupServoModes(int servoMode);
    void setupFunctions();

    bool checkVoltages();

    ////////////////////////////////////////////////////////////////////////////

    unsigned char legMapping[LEGS];

    // Output angles
    float l1[4];
    float l2[4];
    float l3[4];

    ////////////////////////////////////////////////////////////////////////////

    // Enabling/disabling metabot movments
    bool moving = true;

    // Legs backward mode
    bool backLegs = false;

    // Body backward mode
    bool back = false;

    // Time
    float t = 0.0;

    // Speed factor / Time factor gain
    float freq = 2.0;

    // Amplitude & altitude of the robot / Height of the steps / max 35...
    float alt = 15.0;

    // Static position / Robot size & height
    float r = 90.0;
    float h = -55.0;

    // Direction vector
    float dx = 0.0;
    float dy = 0.0;
    float crab = 0.0;
    // Turning, in ° per step
    float turn = 0.0;

    // Front delta H
    float frontH = 0.0;

    float smoothBackLegs = 0.0;
    float smoothBack = -1.0;

    // Average voltage (dV)
    int idToRead = 0;
    double voltageLimit = 6.5;
    double voltageAvg = 8.0;
    double voltageMatrix[LEGS] = {0.0};

    // Gait mode selector
    int gait = GAIT_DEFAULT;

    // Functions
    Function rise;
    Function step;

public:
    Metabot();
    ~Metabot();

    bool setup();

    /*!
     * Computing the servo values
     */
    void run();

    /*!
     * \brief Transmit a move order to the Metabot.
     * \param x: in mm per step, the move in the front axis
     * \param y: in mm per step, the move in the lateral axis
     * \param t: in ° per step, the rotation of the robot
     *
     * Other control options are:
     * backLegs: using legs backward (the small part that is usually up comes below)
     * back: using the robot back, you can then return the robot and using it in the other side, if your electronics allow that
     * freq: the frequency of the walk, default is 2 (for 2hz, or 2 cycles per second)
     * You can change the posture of the robot, BE CAREFUL, this can really mess up if you pass too big values here:
     * h: the height of the legs when stopped, default to -70mm
     * r: the radius of the legs (distance to between arm of first servo of each leg and arm of the last servo) default to 80mm
     */
    void move(float x, float y, float t);
    void heightUp();
    void heightDown();

    void move(MoveitMoveit &move);

    /*!
     * Changes the mapping of the leg, the input is the direction (between 0 and 3)
     */
    void legRemap(int direction = 0);

    /*!
     * Colorizes the two front legs
     */
    void legColorize(int color_front = LED_WHITE, int color_back = LED_GREEN);

    void toggleBackLegs();
    void toggleCrabMode();
    void toggleGait();
};

#endif // METABOT_CONTROL_H
