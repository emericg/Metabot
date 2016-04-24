/*!
 * The Metabot project is under CC BY-NC license: http://creativecommons.org/licenses/by-nc/3.0/
 * The authors are:
 * - Robot Campus
 * - Rhoban System S.A.S
 */

#ifndef METABOT_KINEMATIC_H
#define METABOT_KINEMATIC_H

// Math utils
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define RAD2DEG(a) ((a)*180.0/M_PI)
#define DEG2RAD(a) ((a)*M_PI/180.0)

/*!
 * Computes the inverse kinematics for one leg.
 *
 * Input x, y and z are the targe cartesian position,
 * in the referencial of the leg. The z axis is upward,
 * the x axis is alighed with the "0" angle of the first
 * servo.
 *
 * Output are a, b and c, angles for the three servos,
 * the first controls the yaw of the leg, through the
 * motor fixed on the body, and b and c controls the
 * length of the leg and the position of the end of
 * the leg.
 *
 * l1, l2 and l3 are the length between the first motor
 * arm and the second motor arm, between the second and thr
 * third motor arm, and between the third motor arm and the
 * end of the leg.
 *
 *                  c
 *             l2  / \
 *                /   \  l3
 * body -a--l1--b      \
 *                      \
 *                       -
 *             (x, y, z) end of the leg
 */
bool computeIK(float x, float y, float z,
               float *a, float *b, float *c,
               float l1, float l2, float l3);

#endif // METABOT_KINEMATIC_H