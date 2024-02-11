#include <cmath>
#include <tuple>
#include <gonio_funcs.h>
#include <doublependulum.h>


DoublePendulum::DoublePendulum(float _x_o, float _y_o)
{
	x_o = _x_o;
	y_o = _y_o;
	gravitationalConstant = 9.8;
	dampingFactor = 0.0;
	theta1 = 2.09;
	theta1Dot = 0.0;
	theta1DoubleDot = 0.0;
	lengthBob1 = 200.0;
	radiusBob1 = 12.0;
	massBob1 = 10.0;
	theta2 = -3.14;
	theta2Dot = 0.0;
	theta2DoubleDot = 0.0;
	lengthBob2 = 100.0;
	radiusBob2 = 8.0;
	massBob2 = 5.0;
	xBob1 = 0.0;
	yBob1 = 0.0;
	xBob2 = 0.0;
	yBob2 = 0.0;
}

DoublePendulum::DoublePendulum()
{
}

std::tuple<float, float, float, float, float, float> DoublePendulum::getInitial()
{
	return {theta1, lengthBob1, radiusBob1, theta2, lengthBob2, radiusBob2};
}

std::tuple<float, float, float, float> DoublePendulum::calcPositions()
{
	float x, y;
	std::tie(x, y) = gonio_funcs::calcXY(lengthBob1, theta1);
	xBob1 = x_o + x;
	yBob1 = y_o + y;
	std::tie(x, y) = gonio_funcs::calcXY(lengthBob2, theta2);
	xBob2 = xBob1 + x;
	yBob2 = yBob1 + y;
	return {xBob1, yBob1, xBob2, yBob2};
}

void DoublePendulum::updateThetaBob1(float x, float y)
{
	theta1 = gonio_funcs::calcTheta(x - x_o, y - y_o, 0.0);
}

void DoublePendulum::updateThetaBob2(float x, float y)
{
	theta2 = gonio_funcs::calcTheta(x - xBob1, y - yBob1, 0.0);
}

void DoublePendulum::calcThetaDoubleDot()
/*
definition of ordinary differential equation for a double pendulum
*/
{
	float num1, num2, num3, num4, den;
	num1 = -gravitationalConstant * (2 * massBob1 + massBob2) * sin(theta1);
	num2 = -massBob2 * gravitationalConstant * sin(theta1 - 2 * theta2);
	num3 = -2 * sin(theta1 - theta2) * massBob2;
	den = lengthBob1 * (2 * massBob1 + massBob2 - massBob2 * cos(2 * theta1 - 2 * theta2));
	theta1DoubleDot = (num1 + num2 + num3 * num4) / den;

	num1 = 2 * sin(theta1 - theta2);
	num2 = (theta1Dot * theta1Dot * lengthBob1 * (massBob1 + massBob2));
	num3 = gravitationalConstant * (massBob1 + massBob2) * cos(theta1);
	den = lengthBob2 * (2 * massBob1 + massBob2 - massBob2 * cos(2 * theta1 - 2 * theta2));
	theta2DoubleDot = (num1 * (num2 + num3 + num4)) / den;
}

void DoublePendulum::calcThetaDot(float deltaTime)
/*
create a generator that solves the ODE by calculating the integral and yielding time(in ms) and theta.
*/
{
	this->calcThetaDoubleDot();
	theta1Dot += theta1DoubleDot * deltaTime * (1.0 - dampingFactor);
	theta2Dot += theta2DoubleDot * deltaTime * (1.0 - dampingFactor);
	theta1 += theta1Dot * deltaTime;
	theta2 += theta2Dot * deltaTime;

}
