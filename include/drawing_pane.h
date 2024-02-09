#ifndef BASICDRAWPANE1_H
#define BASICDRAWPANE1_H
#include <wx/wx.h>

class CircleObject;

class BasicDrawPane : public wxPanel
{
public:
	bool dragEnabled;
	CircleObject *fixedCircle;
	CircleObject *movingCircle;

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

class CircleObject
{
public:
	int x, y, lineWidth;
	float radius;
	BasicDrawPane *bdpPointer;
	const wxBrush *brushFillColorPointer;
	CircleObject(BasicDrawPane &_bdp, int _x, int _y, float _radius, const wxBrush *_brushFillColorPointer, int _lineWidth);
	CircleObject();
	void draw();
	void update(int x, int y);
	bool mouseHover(int x, int y);
};

#endif // BASICDRAWPANE1_H