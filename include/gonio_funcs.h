#ifndef GONIOGUNCS_H
#define GONIOFUNCS_H
#include <tuple>

namespace gonio_funcs
{

float dist(float x1, float y1, float x2, float y2); // returns pythagoras distance

float calcTheta(float x, float y, float theta); // returns atan(x, y)

std::tuple<float, float> calcXY(float length, float theta); // returns tuple {x * sin(t), y * cos(t)}

}
#endif // GONIOFUNCS_H