#include <cmath>
#include <wx/wx.h>
#include <drawing_pane.h>

// catch paint events
BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
EVT_PAINT(BasicDrawPane::paintEvent)
EVT_MOTION(BasicDrawPane::mouseMoved)
// EVT_LEFT_DOWN(BasicDrawPane::mouseDown)
// EVT_LEFT_UP(BasicDrawPane::mouseReleased)
EVT_LEFT_DOWN(BasicDrawPane::leftClick)
EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
// EVT_LEAVE_WINDOW(BasicDrawPane::mouseLeftWindow)
// EVT_KEY_DOWN(BasicDrawPane::keyPressed)
// EVT_KEY_UP(BasicDrawPane::keyReleased)
// EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
END_EVENT_TABLE()

BasicDrawPane::BasicDrawPane(wxFrame *parent) : wxPanel(parent)
{
	dragEnabled = false;
	fixedCircle = new CircleObject();
	movingCircle = new CircleObject();
}

BasicDrawPane::BasicDrawPane()
{
}

void BasicDrawPane::leftClick(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	printf("\nleft click @ x: %d, y: %d", pt.x, pt.y);
	movingCircle = new CircleObject(*this, pt.x, pt.y, 10.0, wxYELLOW_BRUSH, 2);
	drawObject(pt.x, pt.y);
	dragEnabled = true;
}

void BasicDrawPane::rightClick(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	printf("\nright click @ x: %d, y: %d", pt.x, pt.y);
	dragEnabled = false;
}

void BasicDrawPane::mouseMoved(wxMouseEvent &event)
{
	if (dragEnabled)
	{
		wxPoint pt = event.GetPosition();
		bool isHover = (*fixedCircle).mouseHover(pt.x, pt.y);
		printf("\nmouse moved @ x: %d, y: %d, hover: %s", pt.x, pt.y, isHover ? "true" : "false");
		drawObject(pt.x, pt.y);
	}
}

void BasicDrawPane::paintEvent(wxPaintEvent &evt)
{
	fixedCircle = new CircleObject(*this, 300, 450, 30.0, wxGREEN_BRUSH, 2);
	render();
}

void BasicDrawPane::render()
{
	// draw fixed items
	(*fixedCircle).draw();
}

void BasicDrawPane::drawObject(int x, int y)
{
	// draw object
	wxClientDC dc(this);
	dc.Clear();
	(*movingCircle).update(x, y);
	(*movingCircle).draw();
	render();
}

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
	float dist = sqrt((x - _x) * (x - _x) + (y - _y) * (y - _y));
	if (dist < radius)
		return true;
	else
		return false;
}
