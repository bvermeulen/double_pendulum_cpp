#ifndef BASICDRAWPANE1_H
#define BASICDRAWPANE1_H
#include <wx/wx.h>

class CircleObject;

class BasicDrawPane : public wxPanel
{
public:
	bool dragEnabled;
	int x_o, y_o;
	CircleObject *fixedCircle, *movingCircle;

public:
	BasicDrawPane(wxFrame *parent);
	BasicDrawPane();
	void paintEvent(wxPaintEvent &evt);
	void render();
	void drawObject(int x, int y);

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