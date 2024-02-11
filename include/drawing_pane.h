#ifndef BASICDRAWPANE1_H
#define BASICDRAWPANE1_H
#include <wx/wx.h>

class CircleObject;
class LineObject;
class DoublePendulum;

class BasicDrawPane : public wxPanel
{
private:
	bool dragBob1Enabled, dragBob2Enabled;
	float x_o, y_o;
	CircleObject *originCircle, *bob1Circle, *bob2Circle;
	LineObject *originLine, *bob1Line, *bob2Line;
	DoublePendulum *dpObject;

public:
	BasicDrawPane(wxFrame *parent);
	BasicDrawPane();
	void paintEvent(wxPaintEvent &evt);
	void animateDoublePendulum();
	void render();
	void drawObject();

	// some useful events
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


#endif // BASICDRAWPANE1_H