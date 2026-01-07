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
	theta1 = -2.741563189;
	theta1Dot = 0.0;
	theta1DoubleDot = 0.0;
	lengthBob1 = 5.0;
	massBob1 = 5.0;
	theta2 = +2.74627558;
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

std::tuple<double, double> DoublePendulum::getRadiusSize()
{
	// implement mass ~ radius ** 3
	return {massBob1 * radiusFactor, massBob2 * radiusFactor};
}

std::tuple<double, double, double, double> DoublePendulum::getPositions()
{
	double x, y;
	std::tie(x, y) = gonio_funcs::calcXY(lengthBob1 * modelFactor, theta1);
	xBob1 = x_o + x;
	yBob1 = y_o + y;
	std::tie(x, y) = gonio_funcs::calcXY(lengthBob2 * modelFactor, theta2);
	xBob2 = xBob1 + x;
	yBob2 = yBob1 + y;
	return {xBob1, yBob1, xBob2, yBob2};
}

std::tuple<double, double, double, double, double> DoublePendulum::getSettings()
{
	return {massBob1, lengthBob1, massBob2, lengthBob2, dampingFactor};
}

void DoublePendulum::setSettings(double _massBob1, double _lengthBob1, double _massBob2, double _lengthBob2, double _dampingFactor)
{
	// if you do not want to change a setting provide -1 as parameter
	if (_massBob1 > 0.0)
		massBob1 = _massBob1;
	if (_lengthBob1 > 0.0)
		lengthBob1 = _lengthBob1;
	if (_massBob2 > 0.0)
		massBob2 = _massBob2;
	if (_lengthBob2 > 0.0)
		lengthBob2 = _lengthBob2;
	if (_dampingFactor > 0.0)
		dampingFactor = _dampingFactor;
}

void DoublePendulum::updateOrigin(double x, double y)
{
	x_o = x;
	y_o = y;
}

void DoublePendulum::updateThetaBob1(double x, double y)
{
	theta1 = gonio_funcs::calcTheta(x - x_o, y - y_o, 0.0);
}

void DoublePendulum::updateThetaBob2(double x, double y)
{
	theta2 = gonio_funcs::calcTheta(x - xBob1, y - yBob1, 0.0);
}

void DoublePendulum::calcThetaDoubleDot(double t1, double t2)
/*
definition of ordinary differential equation for a double pendulum
*/
{
    double _n1, _n2, _n3, _n4;
    double dt = t1 - t2;
    double _sin_dt = sin(dt);
    double _den = (massBob1 + massBob2 * _sin_dt * _sin_dt);

    _n1 = massBob2 * lengthBob1 * theta1Dot * theta1Dot * sin(2*dt);
    _n2 = 2 * massBob2 * lengthBob2 * theta2Dot * theta2Dot * _sin_dt;
    _n3 = 2 * gravitationalConstant * (massBob2 * cos(t2) * _sin_dt + massBob1 * sin(t1));
    _n4 = 2 * (dampingFactor * theta1Dot - dampingFactor * theta2Dot * cos(dt));
    theta1DoubleDot = (_n1 + _n2 + _n3 + _n4)/ (-2 * lengthBob1 * _den);

    _n1 = massBob2 * lengthBob2 * theta2Dot * theta2Dot * sin(2*dt);
    _n2 = 2 * (massBob1 + massBob2) * lengthBob1 * theta1Dot * theta1Dot * _sin_dt;
    _n3 = 2 * gravitationalConstant * (massBob1 + massBob2) * cos(t1) * _sin_dt;
    _n4 = 2 * (dampingFactor * theta1Dot * cos(dt) - dampingFactor * theta2Dot * (massBob1 + massBob2)/ massBob2);
    theta2DoubleDot = (_n1 + _n2 + _n3 + _n4)/ (2 * lengthBob2 *_den);
}

void DoublePendulum::calcThetaDotEuler(double deltaTime)
/*
create a generator that solves the ODE by calculating the integral and yielding time(in ms) and theta, 
using the Euler method
*/
{
	calcThetaDoubleDot(theta1, theta2);
	theta1Dot += theta1DoubleDot * deltaTime;
	theta1 += theta1Dot * deltaTime;
	theta2Dot += theta2DoubleDot * deltaTime;
	theta2 += theta2Dot * deltaTime;
}

void DoublePendulum::calcThetaDotRK4(double deltaTime)
/*
create a generator that solves the ODE by calculating the integral and yielding time(in ms) and theta, 
using the Runge-Kutta 4 method
*/
{
    calcThetaDoubleDot(theta1, theta2);
    double V10 = deltaTime * theta1Dot;
    double A10 = deltaTime * theta1DoubleDot;
    double V20 = deltaTime * theta2Dot;
    double A20 = deltaTime * theta2DoubleDot;

    calcThetaDoubleDot(theta1 + 0.5*V10, theta2 + 0.5*V20);
    double V11 = deltaTime * (theta1Dot + 0.5*A10);
    double A11 = deltaTime * theta1DoubleDot;
    double V21 = deltaTime * (theta2Dot + 0.5*A20);
    double A21 = deltaTime * theta2DoubleDot;

    calcThetaDoubleDot(theta1 + 0.5*V11, theta2 + 0.5*V21);
    double V12 = deltaTime * (theta1Dot + 0.5*A11);
    double A12 = deltaTime * theta1DoubleDot;
    double V22 = deltaTime * (theta2Dot + 0.5*A21);
    double A22 = deltaTime * theta2DoubleDot;

    calcThetaDoubleDot(theta1 + 0.5*V12, theta2 + 0.5*V22);
    double V13 = deltaTime * (theta1Dot + 0.5*A12);
    double A13 = deltaTime * theta1DoubleDot;
    double V23 = deltaTime * (theta2Dot + 0.5*A22);
    double A23 = deltaTime * theta2DoubleDot;

    const double den = 1.0 / 6.0;
    theta1 += den * (V10+2.0*V11+2.0*V12+V13);
    theta1Dot += den * (A10+2.0*A11+2.0*A12+A13);
    theta2 += den * (V20+2.0*V21+2.0*V22+V23);
    theta2Dot += den * (A20+2.0*A21+2.0*A22+A23);
}

void DoublePendulum::clearThetaDotDoubleDot()
{
	theta1Dot = 0.0;
	theta1DoubleDot = 0.0;
	theta2Dot = 0.0;
	theta2DoubleDot = 0.0;
}
