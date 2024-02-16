#include <cmath>
#include <tuple>
#include <gonio_funcs.h>
#include <doublependulum.h>

DoublePendulum::DoublePendulum()
{
	x_o = 0.0;
	y_o = 0.0;
	modelFactor = 25;
	radiusFactor = 2.0;
	gravitationalConstant = 9.8;
	dampingFactor = 0.0;
	theta1 = 2.09;
	theta1Dot = 0.0;
	theta1DoubleDot = 0.0;
	lengthBob1 = 5.0;
	massBob1 = 5.0;
	theta2 = -3.14;
	theta2Dot = 0.0;
	theta2DoubleDot = 0.0;
	lengthBob2 = 2.50;
	massBob2 = 2.5;
	xBob1 = 0.0;
	yBob1 = 0.0;
	xBob2 = 0.0;
	yBob2 = 0.0;
	std::tie(radiusBob1, radiusBob2) = getRadiusSize();
}

std::tuple<float, float> DoublePendulum::getRadiusSize()
{
	// implement mass ~ radius ** 3
	return {massBob1 * radiusFactor, massBob2 * radiusFactor};
}

std::tuple<float, float, float, float> DoublePendulum::getPositions()
{
	float x, y;
	std::tie(x, y) = gonio_funcs::calcXY(lengthBob1 * modelFactor, theta1);
	xBob1 = x_o + x;
	yBob1 = y_o + y;
	std::tie(x, y) = gonio_funcs::calcXY(lengthBob2 * modelFactor, theta2);
	xBob2 = xBob1 + x;
	yBob2 = yBob1 + y;
	return {xBob1, yBob1, xBob2, yBob2};
}

std::tuple<float, float, float, float, float> DoublePendulum::getSettings()
{
	return {massBob1, lengthBob1, massBob2, lengthBob2, dampingFactor};
}

void DoublePendulum::setSettings(float _massBob1, float _lengthBob1, float _massBob2, float _lengthBob2, float _dampingFactor)
{
	// if you do not want to change a setting provide -1 as parameter
	if (_massBob1 > 0.0) massBob1 = _massBob1;
	if (_lengthBob1 > 0.0) lengthBob1 = _lengthBob1;
	if (_massBob2 > 0.0) massBob2 = _massBob2;
	if (_lengthBob2 > 0.0) lengthBob2 = _lengthBob2;
	if (_dampingFactor > 0.0) dampingFactor = _dampingFactor;
}

void DoublePendulum::updateOrigin(float x, float y)
{
	x_o = x;
	y_o = y;
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
	num4 = theta2Dot * theta2Dot * lengthBob2 + theta1Dot * theta1Dot * lengthBob2 * cos(theta1 - theta2);
	den = lengthBob1 * (2 * massBob1 + massBob2 - massBob2 * cos(2 * theta1 - 2 * theta2));
	theta1DoubleDot = (num1 + num2 + num3 * num4) / den;

	num1 = 2 * sin(theta1 - theta2);
	num2 = (theta1Dot * theta1Dot * lengthBob1 * (massBob1 + massBob2));
	num3 = gravitationalConstant * (massBob1 + massBob2) * cos(theta1);
	num4 = (theta2Dot * theta2Dot * lengthBob2 * massBob2 * cos(theta1 - theta2));
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
