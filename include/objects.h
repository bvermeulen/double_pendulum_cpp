#ifndef OBJECTS_H
#define OBJECTS_H
#include <wx/wx.h>
#include <tuple>

class DrawingPanel;

class CircleObject
{
private:
	int x, y;
	float radius;
	int lineWidth;
	const wxBrush *brushFillColorPointer;
	const wxColor *colorPointer;
	DrawingPanel *drawingPanelPointer;

public:
	CircleObject(
		DrawingPanel *_drawingPanelPointer,
		int _x,
		int _y,
		float _radius,
		const wxBrush *_brushFillColorPointer,
		int _lineWidth,
		const wxColor *_colorPointer);
	void draw();
	void update(int x, int y, float radius);
	bool mouseHover(int x, int y);
	void setColors(const wxBrush *_brushFillColorPointer, const wxColor *_colorPointer);
	std::tuple<const wxBrush *, const wxColor *> getColors();
};

class LineObject
{
private:
	int x1, y1, x2, y2;
	int lineWidth;
	const wxColor *colorPointer;
	DrawingPanel *drawingPanelPointer;

public:
	LineObject(
		DrawingPanel *drawingPanelPointer,
		int _x1,
		int _y1,
		int _x2,
		int _y2,
		int _lineWidth,
		const wxColor *_colorPointer);
	void draw();
	void update(int x1, int y1, int x2, int y2);
};

class TracerObject
{
private:
	wxVector<wxPoint> tracerVector;
	int lineWidth;
	const wxColor *colorPointer;
	DrawingPanel *drawingPanelPointer;

public:
	TracerObject(
		DrawingPanel *_drawingPanelPointer,
		int _lineWidth,
		const wxColor *_colorPointer);
	void draw();
	void update(float x, float y);
	void clear();
};

#endif // OBJECTS_H