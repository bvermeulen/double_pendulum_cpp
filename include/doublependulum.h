#ifndef DOUBLEPENDULUM_H
#define DOUBLEPENDULUM_H
#include <tuple>


class DoublePendulum
{
public:
	float theta1, theta2;

private:
	float x_o, y_o;
	float gravitationalConstant, dampingFactor, radiusFactor, modelFactor;
	float theta1Dot, theta1DoubleDot, lengthBob1, radiusBob1, massBob1;
	float theta2Dot, theta2DoubleDot, lengthBob2, radiusBob2, massBob2;
	float xBob1, yBob1, xBob2, yBob2;


public:
	DoublePendulum();
	std::tuple<float, float> getRadiusSize();
	std::tuple<float, float, float, float> getPositions();
	std::tuple<float, float, float, float, float> getSettings();
	void setSettings(float _massBob1, float _lengthBob1, float _massBob2, float _lengthBob2, float _dampingFactor);
	void updateOrigin(float x, float y);
	void updateThetaBob1(float x, float y);
	void updateThetaBob2(float x, float y);
	void calcThetaDot(float deltaTime);
	void calcThetaDoubleDot();
};

#endif // DOUBLEPENDULUM_H
