#include <wx/wx.h>
#include <gonio_funcs.h>
#include <drawing_pane.h>
#include <objects.h>

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
	x_o = 520;
	y_o = 160;
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
		//printf("\nmouse moved @ x: %d, y: %d, hover: %s", pt.x, pt.y, isHover ? "true" : "false");

		float theta = gonio_funcs::calcTheta(pt.x - x_o, pt.y - y_o, 0.0);
		auto [x, y] = gonio_funcs::calcXY(10.0, theta);
		printf("\ntheta: %.2f, x: %.0f, y: %.0f", theta, x, y);
		drawObject(pt.x, pt.y);
	}
}

void BasicDrawPane::paintEvent(wxPaintEvent &evt)
{
	fixedCircle = new CircleObject(*this, x_o, y_o, 5.0, wxBLACK_BRUSH, 2);
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

