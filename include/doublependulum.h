#ifndef DOUBLEPENDULUM_H
#define DOUBLEPENDULUM_H
#include <tuple>


class DoublePendulum
{
public:
	double theta1, theta2;

private:
	double x_o, y_o;
	double gravitationalConstant, dampingFactor, radiusFactor, modelFactor;
	double theta1Dot, theta1DoubleDot, lengthBob1, radiusBob1, massBob1;
	double theta2Dot, theta2DoubleDot, lengthBob2, radiusBob2, massBob2;
	double xBob1, yBob1, xBob2, yBob2;
	void calcThetaDoubleDot(double t1, double t2);
	
	
	public:
	DoublePendulum();
	std::tuple<double, double> getRadiusSize();
	std::tuple<double, double, double, double> getPositions();
	std::tuple<double, double, double, double, double> getSettings();
	void setSettings(double _massBob1, double _lengthBob1, double _massBob2, double _lengthBob2, double _dampingFactor);
	void updateOrigin(double x, double y);
	void updateThetaBob1(double x, double y);
	void updateThetaBob2(double x, double y);
	void calcThetaDotEuler(double deltaTime);
	void calcThetaDotRK4(double deltaTime);
	void clearThetaDotDoubleDot();
};

#endif // DOUBLEPENDULUM_H
