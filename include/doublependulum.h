#ifndef DOUBLEPENDULUM_H
#define DOUBLEPENDULUM_H
#include <tuple>
#include <vector>
#include <boost/numeric/odeint.hpp>


typedef std::vector<double> state_type;

class HarmOscillator
{
    public:
        HarmOscillator(
            double b1Weight, double r1Length,
            double b2Weight, double r2Length,
            double df1, double df2, double g            
        );
        ~HarmOscillator();
        void operator() (const state_type &thetaState, state_type &thetaDotState, const double t);

    private:
        double bob1Weight;        
        double rod1Length;
        double bob2Weight;
        double rod2Length;    
        double dampingFactor1;
        double dampingFactor2;
		double gravitationalConstant;
};

class DoublePendulum
{
	private:
		double theta1, theta2;
		double time;
		double gravitationalConstant, dampingFactor;
		double theta1Dot, theta1DoubleDot, lengthBob1, radiusBob1, massBob1;
		double theta2Dot, theta2DoubleDot, lengthBob2, radiusBob2, massBob2;
		double xBob1, yBob1, xBob2, yBob2;
		void calcThetaDoubleDot(double t1, double t2);
		HarmOscillator *ho;
		state_type thetaState;
        boost::numeric::odeint::runge_kutta4<state_type> rk4;        

	
	public:
		DoublePendulum();
		~DoublePendulum();
		std::tuple<double, double> getRadiusSize();
		std::tuple<double, double, double, double> getPositions();
		std::tuple<double, double, double, double, double> getSettings();
		void setSettings(double _massBob1, double _lengthBob1, double _massBob2, double _lengthBob2, double _dampingFactor);
		void updateThetaBob1(double x, double y);
		void updateThetaBob2(double x, double y);
		void calcThetaDotEuler(double deltaTime);
		void calcThetaDotRK4(double deltaTime);
		void calcThetaDotBoost(double deltaTime);	
		void clearThetaDotDoubleDot();
};

#endif // DOUBLEPENDULUM_H
