#include <cmath>
#include <tuple>

namespace gonio_funcs
{

float dist(int x1, int x2, int y1, int y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float calcTheta(float x, float y, float theta)
{
	if ((y == 0.0) && (x == 0.0))
		return theta;
	else
		return atan2f(x, -y);
}

std::tuple<float, float> calcXY(float length, float theta)
{
	float x = length * sin(theta);
	float y = length * cos(theta);
	return {x, y};
}

}
