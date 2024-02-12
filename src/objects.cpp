#include <wx/wx.h>
#include <objects.h>
#include <gonio_funcs.h>
#include <drawing_pane.h>

CircleObject::CircleObject(
	BasicDrawPane &_bdp,
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
	bdpPointer = &_bdp;
}
CircleObject::CircleObject()
{
}

void CircleObject::draw()
{
	wxClientDC dc(bdpPointer);
	dc.SetBrush(*brushFillColorPointer);
	dc.SetPen(wxPen(*colorPointer, 2));
	dc.DrawCircle(wxPoint(x, y), radius);
}

void CircleObject::update(int _x, int _y)
{
	x = _x;
	y = _y;
}

bool CircleObject::mouseHover(int _x, int _y)
{
	if (gonio_funcs::dist(x, y, _x, _y) < radius)
		return true;
	else
		return false;
}

LineObject::LineObject(
	BasicDrawPane &_bdp,
	int _x1,
	int _y1,
	int _x2,
	int _y2,
	int _lineWidth,
	const wxColor *_colorPointer)
{
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
	lineWidth = _lineWidth;
	colorPointer = _colorPointer;
	bdpPointer = &_bdp;
}

LineObject::LineObject()
{
}

void LineObject::draw()
{
	wxClientDC dc(bdpPointer);
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
	BasicDrawPane &_bdp,
	int _lineWidth,
	const wxColor *_colorPointer)
{
	lineWidth = _lineWidth;
	colorPointer = _colorPointer;
	bdpPointer = &_bdp;
}

TracerObject::TracerObject()
{
}

void TracerObject::draw()
{
	unsigned int size;
	wxClientDC dc(bdpPointer);
	dc.SetPen(wxPen(*colorPointer, lineWidth));
	size = tracerVector.size();
	if (size > 0) dc.DrawLines(size, &tracerVector[0]);
	if (size % 100 == 0) printf("\ntracer vector size: %8lld", tracerVector.size());
}

void TracerObject::update(float x, float y)
{
	tracerVector.push_back(wxPoint((int)x, (int)(y)));
}
