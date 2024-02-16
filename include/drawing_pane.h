#ifndef DRAWINGPANE_H
#define DRAWINGPANE_H
#include <wx/wx.h>
#include <tuple>

class CircleObject;
class LineObject;
class TracerObject;
class DoublePendulum;

class DrawingPane : public wxPanel
{
private:
	bool dragBob1Enabled, dragBob2Enabled, runEnabled, paintEventDone, tracerEnabled;
	float x_o, y_o, originLineLength, originSize;
	int blitCount;
	CircleObject *originCircle, *bob1Circle, *bob2Circle;
	LineObject *originLine, *bob1Line, *bob2Line;
	TracerObject *tracerLine;
	DoublePendulum *dpObject;

public:
	enum Control
	{
		START,
		STOP,
		TOGGLETRACE,
		CLEARTRACE,
		SWITCHCOLOR
	};
	DrawingPane(wxFrame *parent);
	void paintEvent(wxPaintEvent &evt);
	void onSize(wxSizeEvent &event);
	void animateDoublePendulum();
	void controlAction(Control control);
	std::tuple<float, float, float, float, float> getSettings();
	void setSettings(float _massBob1, float _lengthBob1, float _massBob2, float _lengthBob2, float _dampingFactor);
	void updateObjects();
	void drawObjects();
	void mouseMoved(wxMouseEvent &event);
	// void mouseDown(wxMouseEvent &event);
	// void mouseWheelMoved(wxMouseEvent &event);
	// void mouseReleased(wxMouseEvent &event);
	void leftClick(wxMouseEvent &event);
	void rightClick(wxMouseEvent &event);
	// void mouseLeftWindow(wxMouseEvent &event);
	// void keyPressed(wxKeyEvent &event);
	// void keyReleased(wxKeyEvent &event);

	DECLARE_EVENT_TABLE()
};

#endif // DRAWINGPANE_H