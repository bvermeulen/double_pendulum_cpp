#ifndef DOUBLEPENDULUM_H
#define DOUBLEPENDULUM_H
#include <tuple>


class DoublePendulum
{
public:
	float x_o, y_o;
	float gravitationalConstant, dampingFactor;
	float theta1, theta1Dot, theta1DoubleDot, lengthBob1, radiusBob1, massBob1;
	float theta2, theta2Dot, theta2DoubleDot, lengthBob2, radiusBob2, massBob2;
	float xBob1, yBob1, xBob2, yBob2;

public :
	DoublePendulum(float x_o, float y_o);
	DoublePendulum();
	std::tuple<float, float, float, float, float, float> getInitial();
	std::tuple<float, float, float, float> calcPositions();
	void updateThetaBob1(float x, float y);
	void updateThetaBob2(float x, float y);
	void calcThetaDot(float deltaTime);
	void calcThetaDoubleDot();

};

#endif // DOUBLEPENDULUM_H
