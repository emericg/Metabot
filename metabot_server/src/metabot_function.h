/*!
 * The Metabot project is under CC BY-NC license: http://creativecommons.org/licenses/by-nc/3.0/
 * The authors are:
 * - Robot Campus
 * - Rhoban System S.A.S
 */

#ifndef METABOT_FUNCTION_H
#define METABOT_FUNCTION_H

class Function
{
public:
    Function();
    ~Function();

    /*!
     * Add a point (x, y) to the function
     */
    void addPoint(double x, double y);

    /*!
     * Gets the xMax of the function
     */
    double getXMax();

    /*!
     * Get the value for the given x
     */
    double get(double x);

    /*!
     * Gets the value for the given x, modulo the size of the function
     */
    double getMod(double x);

    /*!
     * Checks if the function can contain one more element
     */
    void checkSize();

    /*!
     * Reset the number of points
     */
    void clear();

protected:
    /*!
     * Function points
     */
    double *points;

    /*!
     * Number of points
     */
    int nbPoints;

    /*!
     * Array size
     */
    int size;

    /*!
     * The nth item of this array is the slope between the nth point and the
     * n+1th point
     */
    double *ds;
};

#endif // METABOT_FUNCTION_H
