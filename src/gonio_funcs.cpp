#include <cmath>
#include <tuple>

namespace gonio_funcs
{

double dist(double x1, double y1, double x2, double y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

double calcTheta(double x, double y, double theta)
{
	if ((y == 0.0) && (x == 0.0))
		return theta;
	else
		return atan2f(x, y);
}

std::tuple<double, double> calcXY(double length, double theta)
{
	double x = length * sin(theta);
	double y = length * cos(theta);
	return {x, y};
}

}
