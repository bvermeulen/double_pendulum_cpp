#ifndef DRAWINGPANEL_H
#define DRAWINGPANEL_H
#include <wx/wx.h>
#include <wx/stopwatch.h>
#include <wx/event.h>
#include <tuple>

class CircleObject;
class LineObject;
class TracerObject;
class DoublePendulum;

wxDECLARE_EVENT(EVT_UPDATE_VALUES, wxCommandEvent);

class DrawingPanel : public wxPanel
{
	private:
		bool dragBob1Enabled, dragBob2Enabled, runEnabled, paintEventDone, tracerEnabled;
		double x_o, y_o, originLineLength, originSize, modelFactor, radiusFactor; 
		wxStopWatch stopWatch;
		wxLongLong clockTimeMicro;
		long clockTimeMilli;
		long newClockTime_1;
		long newClockTime_2;
		long newClockTime_3;
		long AnimationTime;
		double deltaTime;
		CircleObject *originCircle, *bob1Circle, *bob2Circle;
		LineObject *originLine, *bob1Line, *bob2Line;
		TracerObject *tracerLine;
		DoublePendulum &dpObject;
		std::tuple<double, double, double, double, double, double> fitToPanel(
			double x1, double y1,
			double x2, double y2,
			double r1, double r2
		);
		void animateDoublePendulum();
		void updateObjects();
		void drawObjects();
		void mouseMoved(wxMouseEvent &event);
		void leftClick(wxMouseEvent &event);
		void rightClick(wxMouseEvent &event);
		void paintEvent(wxPaintEvent &event);
		void onSize(wxSizeEvent &event);
		void updateValues();

public:
	enum Control
	{
		START,
		STOP,
		PAUSE,
		RUN,
		TRACE_ON,
		TRACE_OFF,
		TRACE_CLEAR,
		SWITCHCOLOR,
		UPDATE_PANEL
	};
	DrawingPanel(wxFrame *parent, DoublePendulum &dpObjectRef);
	~DrawingPanel();
	void controlAction(Control control);
	long getTime();


};

#endif // DRAWINGPANE_H