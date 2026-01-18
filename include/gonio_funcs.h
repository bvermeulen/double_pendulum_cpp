#ifndef GONIOGUNCS_H
#define GONIOFUNCS_H
#include <tuple>

namespace gonio_funcs
{

    double dist(double x1, double y1, double x2, double y2); // returns pythagoras distance

    double calcTheta(double x, double y, double theta); // returns atan(x, y)

    std::tuple<double, double> calcXY(double length, double theta); // returns tuple {x * sin(t), y * cos(t)}

    double degreeToRad(double angle);

    double radToDegree(double angle);

}
#endif // GONIOFUNCS_H