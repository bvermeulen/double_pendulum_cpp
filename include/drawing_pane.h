#ifndef DRAWINGPANE_H
#define DRAWINGPANE_H
#include <wx/wx.h>

class CircleObject;
class LineObject;
class TracerObject;
class DoublePendulum;

class DrawingPane : public wxPanel
{
private:
	bool dragBob1Enabled, dragBob2Enabled, runEnabled;
	float x_o, y_o;
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
	void animateDoublePendulum();
	void controlAction(Control control);
	void render();
	void drawObject();
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