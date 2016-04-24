/*!
 * The Metabot project is under CC BY-NC license: http://creativecommons.org/licenses/by-nc/3.0/
 * The authors are:
 * - Robot Campus
 * - Rhoban System S.A.S
 */

#include "metabot_kinematic.h"
#include <cmath>

/*!
 * Solves the al Kashi problem.
 * This gives the angles of a triangle side, knowing all the lengths of its sides.
 */
static float alKashi(float a, float b, float c)
{
    return acos(((a*a)+(b*b)-(c*c))/(2*a*b));
}

/*!
 * Compute the kinematics of the robot.
 */
bool computeIK(float x, float y, float z,
               float *a, float *b, float *c,
               float l1, float l2, float l3)
{
    // Alpha is simply the angle of the leg in the X/Y plane
    float alpha = atan2(y, x);

    // Distance between end of the leg and arm of the second motor, in the X/Y plane
    float xp = sqrt(x*x+y*y)-l1;
    if (xp < 0) {
        xp = 0;
    }

    // Distance between second motor arm and the end of the leg, in the plane of the leg
    float d = sqrt(pow(xp,2) + pow(z,2));
    if (d > l2+l3) {
        d = l2+l3;
    }

    // Knowing l2, l3 and d beta and gamma can be computed using the Al Kashi law
    float beta = alKashi(l2, d, l3) - atan2(-z, xp);
    float gamma = M_PI - alKashi(l2, l3, d);

    if (!std::isnan(alpha) && !std::isnan(beta) && !std::isnan(gamma)) {
        *a = RAD2DEG(alpha);
        *b = RAD2DEG(beta);
        *c = RAD2DEG(gamma);
        return true;
    } else {
        return false;
    }
}
