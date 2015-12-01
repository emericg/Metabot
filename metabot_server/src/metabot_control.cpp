/*!
 * The Metabot project is under CC BY-NC license: http://creativecommons.org/licenses/by-nc/3.0/
 * The authors are:
 * - Robot Campus
 * - Rhoban System S.A.S
 */

#include "metabot_control.h"

#include "metabot_function.h"
#include "metabot_kinematic.h"
#include "metabot_config.h"

#include "minitraces.h"



#include <cmath>
#include <cstdlib>

Metabot::Metabot()
{
    // Init voltage matrix
    for (int i = 0; i < 12; i++)
    {
        voltageMatrix[i] = 8;
    }
}

Metabot::~Metabot()
{
    if (dxl != NULL)
    {
        dxl->disconnect();
        delete dxl;
        dxl = NULL;
    }
}

bool Metabot::setup()
{
    bool status = false;

    if (dxl != NULL)
    {
        dxl->disconnect();
        delete dxl;
        dxl = NULL;
    }

    dxl = new DynamixelSimpleAPI(SERVO_XL);
    if (dxl != NULL)
    {
        std::string device = "auto";

        if (dxl->connect(device, 1000000, SERIAL_USB2AX) == 1)
        {
            status = true;

            // Ping servos to confirm metabot detection
            for (int i = 0; i < 12; i++)
            {
                if (dxl->ping(servos_order[i]) == false)
                {
                    TRACE_WARNING(BOT,  "Warning, servo nb:%i / id: %i is missing!\n", i, servos_order[i]);
                    status = false;

                    // Disconnecting...
                    dxl->disconnect();
                }
            }

            if (status == true)
            {
                // Initializing servo mapping
                legRemap(initialOrientation);

                // Orientation
                back = (initialOrientation != 0);
                if (back) smoothBack = 1;

                // Lower serial timer (lower only after the pings)
                dxl->serialSetLatency(8);
/*
                // Setup limits
                setupLimits();

                // Initializing positions to 0 ? // Or just read actual values ?
                for (int i=0; i<12; i++)
                {
                    dxl->setGoalPosition(servos_order[i], angle_to_step(0.0));
                }
                for (int i=0; i<4; i++)
                {
                    l1[i] = l2[i] = l3[i] = 0.0;
                }
*/
            }
        }
    }

    return status;
}

void Metabot::setupLimits()
{
    if (dxl != NULL)
    {
        // Set servos limits
        dxl->setSetting(BROADCAST_ID, REG_P_GAIN, 64);

        // Set position limits for each legs
        for (int i = 0; i <= 9; i += 3)
        {
            dxl->setMinMaxPositions(servos_order[0+i], angle_to_step(-90), angle_to_step(90));
            dxl->setMinMaxPositions(servos_order[1+i], angle_to_step(-100), angle_to_step(100));
            dxl->setMinMaxPositions(servos_order[2+i], angle_to_step(-145), angle_to_step(145));
        }
    }
}

void Metabot::setupFunctions()
{
    rise.clear();
    step.clear();

    if (gait == GAIT_WALK)
    {
        // Rising the legs
        rise.addPoint(0.0, 0.0);
        rise.addPoint(0.1, 1.0);
        rise.addPoint(0.3, 1.0);
        rise.addPoint(0.35, 0.0);
        rise.addPoint(1.0, 0.0);

        // Taking the leg forward
        step.addPoint(0.0, -0.5);
        step.addPoint(0.12, -0.5);
        step.addPoint(0.3, 0.5);
        step.addPoint(0.35, 0.5);
        step.addPoint(1.0, -0.5);
    }

    if (gait == GAIT_TROT)
    {
        // Rising the legs
        rise.addPoint(0.0, 0.0);
        rise.addPoint(0.1, 1.0);
        rise.addPoint(0.4, 1.0);
        rise.addPoint(0.5, 0.0);
        rise.addPoint(1.0, 0.0);

        // Taking the leg forward
        step.addPoint(0.0, -0.5);
        step.addPoint(0.1, -0.5);
        step.addPoint(0.4, 0.5);
        step.addPoint(1.0, -0.5);
    }
}

int Metabot::angle_to_step(float angle)
{
    return ((angle / 300.0) * 1024) + 512;
}

float Metabot::step_to_angle(int step)
{
    return ((step - 512) / 1024.0) * 300.0;
}

void Metabot::legColorize(int front_color, int back_color)
{
    if (dxl != NULL)
    {
        // Front legs
        for (int i = 0; i < 6; i++)
        {
            dxl->setLed(legMapping[i], 1, front_color);
        }

        // Back legs
        for (int i = 6; i < 12; i++)
        {
            dxl->setLed(legMapping[i], 1, back_color);
        }
    }
}

void Metabot::legRemap(int direction)
{
    // Change leg mapping
    for (int i = 0; i < 12; i++)
    {
        legMapping[i] = servos_order[(i + (3 * direction)) % 12];
    }

    // Need to re-colorize legs
    legColorize();
}

void Metabot::toggleBackLegs()
{
    if (backLegs == 0)
    {
        backLegs = 1;
    }
    else if (backLegs == 1)
    {
        backLegs = 0;
    }
}

void Metabot::toggleCrabMode()
{
    if (crab == 0)
    {
        crab = 30;
    }
    else if (crab == 30)
    {
        crab = 0;
    }
}

void Metabot::toggleGait()
{
    if (gait == GAIT_WALK)
    {
        gait = GAIT_TROT;
        alt = 15.0;
        legColorize(LED_WHITE, LED_GREEN);
    }
    else if (gait == GAIT_TROT)
    {
        gait = GAIT_WALK;
        alt = 35.0;
        legColorize(LED_BLUE, LED_GREEN);
    }
}

#define SPEEDCONTROL 1.3

void Metabot::move(float x, float y, float t)
{
    dx = x * SPEEDCONTROL;
    dy = y * SPEEDCONTROL;
    turn = t * SPEEDCONTROL;
}

void Metabot::move(MoveitMoveit &move)
{
    dx = move.dx * SPEEDCONTROL;
    dy = move.dy * SPEEDCONTROL;
    turn = move.turn * SPEEDCONTROL;

    if (move.height > 0)
        heightUp();
    else if (move.height < 0)
        heightDown();

    if (move.walk)
    {
        gait = GAIT_WALK;
        alt = 35.0;
        legColorize(LED_BLUE, LED_GREEN);
    }
    else
    {
        gait = GAIT_TROT;
        alt = 15.0;
        legColorize(LED_WHITE, LED_GREEN);
    }

    if (move.crab)
        crab = 30;
    else
        crab = 0;

    if (move.inverted)
        backLegs = 1;
    else
        backLegs = 0;
}

void Metabot::heightUp()
{
    if (h >= -155.0)
    {
        h -= 5;
    }
}

void Metabot::heightDown()
{
    if (h <= -40.0)
    {
        h += 5;
    }
}

void Metabot::run()
{
    if (dxl != NULL)
    {
        // Computing average voltage
        {
            if (idToRead >= 12)
                idToRead = 1;

            double avg = 0.0;
            double volt = dxl->readCurrentVoltage(legMapping[idToRead]);

            if (volt > 0.0)
                voltageMatrix[idToRead] = volt;

            for (int i = 0; i < 12; i++)
            {
                avg += voltageMatrix[i];
            }
            voltageAvg = (avg / 12.0);

            if (voltageAvg < voltageLimit)
            {
                TRACE_WARNING(BOT, "Average servo voltage is %fv, below limit of %fv...\n", voltageAvg, voltageLimit);

                // Disable torque
                //dxl_write_word(DXL_BROADCAST, DXL_GOAL_TORQUE, 0); // DEPRECATED
                dxl->setSetting(BROADCAST_ID, REG_TORQUE_LIMIT, 0); // setGoalTorque()

                // LED blink
                dxl->setLed(BROADCAST_ID, idToRead<6, LED_RED);

                moving = false;
            }

            idToRead++;
        }

        if (moving == false)
        {
            t = 0.0;
            return;
        }

        // Setting up functions
        setupFunctions();

        // Incrementing and normalizing t
        t += freq * 0.02;
        if (t > 1.0)
        {
            t -= 1.0;
        }
        if (t < 0.0)
        {
            t += 1.0;
        }

        // Smoothing 180
        if (backLegs && smoothBackLegs < 1)
        {
            smoothBackLegs += 0.02;
        }
        if (!backLegs && smoothBackLegs > 0)
        {
            smoothBackLegs -= 0.02;
        }
        if (back && smoothBack < 1)
        {
            smoothBack += 0.04;
        }
        if (!back && smoothBack > -1)
        {
            smoothBack -= 0.04;
        }

        for (int i = 0; i < 4; i++)
        {
            // Defining in which group of opposite legs this leg is
            bool group = ((i&1)==1);

            // This defines the phase of the gait
            float legPhase;

            if (gait == GAIT_WALK)
            {
                float phases[] = {0.0, 0.5, 0.75, 0.25};
                legPhase = t + phases[i];
            }
            if (gait == GAIT_TROT)
            {
                legPhase = t + group*0.5;
            }

            float x, y, z, a, b, c;

            // Computing the order in the referencial of the body
            float xOrder = step.getMod(legPhase)*dx;
            float yOrder = step.getMod(legPhase)*dy;

            // Computing the order in the referencial of the leg
            float bodyAngle = -(i*M_PI/2.0 - (M_PI/4.0))*smoothBack;
            if (group) {
                bodyAngle -= DEG2RAD(crab*(-smoothBack));
            } else {
                bodyAngle += DEG2RAD(crab*(-smoothBack));
            }
            float vx = xOrder*cos(bodyAngle)-yOrder*sin(bodyAngle);
            float vy = xOrder*sin(bodyAngle)+yOrder*cos(bodyAngle);

            float enableRise = (abs(dx)>0.5 || abs(dy)>0.5 || abs(turn)>5) ? 1 : 0;

            // This is the x,y,z order in the referencial of the leg
            x = r + vx;
            y = vy;
            z = h + rise.getMod(legPhase)*alt*enableRise;
            if (i < 2) z += frontH;

            // Computing inverse kinematics
            if (computeIK(x, y, z, &a, &b, &c, LEG1, LEG2, backLegs ? LEG3_b : LEG3_a))
            {
                if (group)
                {
                    a += crab*(-smoothBack);
                }
                else
                {
                    a -= crab*(-smoothBack);
                }

                l1[i] = signs[0]*smoothBack*(a + step.getMod(legPhase)*turn);
                l2[i] = signs[1]*smoothBack*(b);
                l3[i] = signs[2]*smoothBack*(c - 180*smoothBackLegs);
            }
        }

        // Sending order to servos
        dxl->setGoalPosition(legMapping[0], angle_to_step(l1[0]));
        dxl->setGoalPosition(legMapping[3], angle_to_step(l1[1]));
        dxl->setGoalPosition(legMapping[6], angle_to_step(l1[2]));
        dxl->setGoalPosition(legMapping[9], angle_to_step(l1[3]));

        dxl->setGoalPosition(legMapping[1], angle_to_step(l2[0]));
        dxl->setGoalPosition(legMapping[4], angle_to_step(l2[1]));
        dxl->setGoalPosition(legMapping[7], angle_to_step(l2[2]));
        dxl->setGoalPosition(legMapping[10], angle_to_step(l2[3]));

        dxl->setGoalPosition(legMapping[2], angle_to_step(l3[0]));
        dxl->setGoalPosition(legMapping[5], angle_to_step(l3[1]));
        dxl->setGoalPosition(legMapping[8], angle_to_step(l3[2]));
        dxl->setGoalPosition(legMapping[11], angle_to_step(l3[3]));
    }
}
