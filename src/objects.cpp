#include <wx/wx.h>
#include <objects.h>
#include <gonio_funcs.h>
#include <drawing_pane.h>

CircleObject::CircleObject(BasicDrawPane &_bdp, int _x, int _y, float _radius, const wxBrush *_brushFillColorPointer, int _lineWidth)
{
	x = _x;
	y = _y;
	radius = _radius;
	lineWidth = _lineWidth;
	brushFillColorPointer = _brushFillColorPointer;
	bdpPointer = &_bdp;
}
CircleObject::CircleObject()
{
}

void CircleObject::draw()
{
	wxClientDC dc(bdpPointer);
	dc.SetBrush(*brushFillColorPointer);
	dc.SetPen(wxPen(wxColor(150, 150, 250), lineWidth));
	dc.DrawCircle(wxPoint(x, y), radius);
}

void CircleObject::update(int _x, int _y)
{
	x = _x;
	y = _y;
}

bool CircleObject::mouseHover(int _x, int _y)
{
	if (gonio_funcs::dist(x, _x, y, _y) < radius)
		return true;
	else
		return false;
}
