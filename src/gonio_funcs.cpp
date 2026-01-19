#define _USE_MATH_DEFINES
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

	double degreeToRad(double angle)
	{
		angle = angle * (M_PI / 180.0);
		if (angle >= M_PI)
		{
			angle -= 2 * M_PI;
		}
		if (angle < -M_PI)
		{
			angle += 2 * M_PI;
		}
		return angle;
	}

	double radToDegree(double angle)
	{
		angle = angle * (180.0 / M_PI);
		angle = fmod(angle, 360.0);
		if (angle >= 180.)
		{
			angle -= 360.0;
		}
		if (angle < -180.0)
		{
			angle += 360.0;
		}
		return angle;
	}
}
