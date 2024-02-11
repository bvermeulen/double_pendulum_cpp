#ifndef GONIOGUNCS_H
#define GONIOFUNCS_H

namespace gonio_funcs
{

float dist(int x1, int x2, int y1, int y2); // returns pythagoras distance

float calcTheta(float x, float y, float theta); // returns atan(x, -y)

std::tuple<float, float> calcXY(float length, float theta); // returns tuple {x * sin(t), y * cos(t)}

}
#endif // GONIOFUNCS_H