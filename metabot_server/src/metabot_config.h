/*!
 * The Metabot project is under CC BY-NC license: http://creativecommons.org/licenses/by-nc/3.0/
 * The authors are:
 * - Robot Campus
 * - Rhoban System S.A.S
 */

#ifndef METABOT_CONFIG_H
#define METABOT_CONFIG_H

// Legs count
#define LEGS            12

// These are the dimension of each legs, in mm
#define LEG0      35
#define LEG1      32
#define LEG2      60
#define LEG3_a    85
#define LEG3_b    35

/*!
 * Servos are supposed to be like this:
 *
 *   2nd leg   3
 *         |   2 <-- 1st leg
 *         v   1
 *       6-5-4   10-11-11
 *             7    ^
 * 3rd leg --> 8    |
 *             9    4th leg
 *
 * But you can change these IDs using the below array:
 */
static int servos_order[] = {
    // First leg
    1, 2, 3,
    // Second leg
    4, 5, 6,
    // Third leg
    7, 8, 9,
    // Fourth leg
    10, 11, 12
};

/*!
 * Chooses the initial orientation of the spider, 0 means first and second legs
 * are front legs. Set to 0 if you want it to be initialized in the opposite
 * orientation (third and fourth as front legs).
 */
static int initialOrientation = 0;

/*!
 * You can also change the signs of the servos if you mount
 * it backward for instance
 *
 * Change the 1 to -1 if the servos are backwards
 */
static int signs[] = {
    1,          // Signs of the body motors
    1,          // Sign of the middle-leg motors
    1           // Sign of the end-legs motors
};

#endif // METABOT_CONFIG_H
