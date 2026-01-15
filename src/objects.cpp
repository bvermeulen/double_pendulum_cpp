#include <wx/wx.h>
#include <main_panel.h>
#include <objects.h>
#include <gonio_funcs.h>

CircleObject::CircleObject(
	MainPanel *_drawingPanelPointer,
	int _x,
	int _y,
	float _radius,
	const wxBrush *_brushFillColorPointer,
	int _lineWidth,
	const wxColor *_colorPointer)
{
	x = _x;
	y = _y;
	radius = _radius;
	lineWidth = _lineWidth;
	brushFillColorPointer = _brushFillColorPointer;
	colorPointer = _colorPointer;
	mainPanelPointer = _drawingPanelPointer;
}

void CircleObject::draw()
{
	wxClientDC dc(mainPanelPointer);
	dc.SetBrush(*brushFillColorPointer);
	dc.SetPen(wxPen(*colorPointer, 2));
	dc.DrawCircle(wxPoint(x, y), radius);
}

void CircleObject::update(int _x, int _y, float _radius)
{
	x = _x;
	y = _y;
	radius = _radius;
}

bool CircleObject::mouseHover(int _x, int _y)
{
	if (gonio_funcs::dist(x, y, _x, _y) < radius)
		return true;
	else
		return false;
}

void CircleObject::setColors(const wxBrush *_brushFillColorPointer, const wxColor *_colorPointer)
{
	brushFillColorPointer = _brushFillColorPointer;
	colorPointer = _colorPointer;
}

std::tuple<const wxBrush *, const wxColor *> CircleObject::getColors()
{
	return {brushFillColorPointer, colorPointer};
}


LineObject::LineObject
(
	MainPanel *_drawingPanelPointer,
	int _x1,
	int _y1,
	int _x2,
	int _y2,
	int _lineWidth,
	const wxColor *_colorPointer
)
{
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
	lineWidth = _lineWidth;
	colorPointer = _colorPointer;
	drawingPanelPointer = _drawingPanelPointer;
}

void LineObject::draw()
{
	wxClientDC dc(drawingPanelPointer);
	dc.SetPen(wxPen(*colorPointer, lineWidth));
	dc.DrawLine(x1, y1, x2, y2);
}

void LineObject::update(int _x1, int _y1, int _x2, int _y2)
{
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
}

TracerObject::TracerObject(
	MainPanel *_drawingPanelPointer,
	int _lineWidth,
	const wxColor *_colorPointer)
{
	lineWidth = _lineWidth;
	colorPointer = _colorPointer;
	drawingPanelPointer = _drawingPanelPointer;
}

void TracerObject::draw()
{
	wxClientDC dc(drawingPanelPointer);
	dc.SetPen(wxPen(*colorPointer, lineWidth));
	unsigned int size = getSize();
	if (size > 0)
		dc.DrawLines(size, &tracerVector[0]);
}

void TracerObject::update(float x, float y)
{
	tracerVector.push_back(wxPoint((int)x, (int)(y)));
}

void TracerObject::clear()
{
	tracerVector.clear();
}

unsigned int TracerObject::getSize()
{
	return tracerVector.size();
}