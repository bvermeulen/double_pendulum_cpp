#include <cstdio>
#include <tuple>
#include <wx/wx.h>
#include <gonio_funcs.h>
#include <doublependulum.h>
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
	x_o = 520;
	y_o = 360;
	dragBob1Enabled = false;
	dragBob2Enabled = false;
	dpObject = new DoublePendulum(x_o, y_o);
	originCircle = new CircleObject();
	originLine = new LineObject();
	bob1Circle = new CircleObject();
	bob2Circle = new CircleObject();
	bob1Line = new LineObject();
	bob2Line = new LineObject();
}

BasicDrawPane::BasicDrawPane()
{
}

void BasicDrawPane::leftClick(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	printf("\nleft click @ x: %d, y: %d", pt.x, pt.y);
	if (bob1Circle->mouseHover(pt.x, pt.y))
		dragBob1Enabled = true;
	if (bob2Circle->mouseHover(pt.x, pt.y))
		dragBob2Enabled = true;
}

void BasicDrawPane::rightClick(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	printf("\nright click @ x: %d, y: %d", pt.x, pt.y);
	dragBob1Enabled = false;
	dragBob2Enabled = false;
}

void BasicDrawPane::mouseMoved(wxMouseEvent &event)
{
	if (dragBob1Enabled || dragBob2Enabled)
	{
		wxPoint pt = event.GetPosition();
		if (dragBob1Enabled)
		{
			dpObject->updateThetaBob1(pt.x, pt.y);
			printf("\nmouse moved @ x: %d, y: %d, theta: %.2f", pt.x, pt.y, dpObject->theta1);
		}
		else
		{
			dpObject->updateThetaBob2(pt.x, pt.y);
			printf("\nmouse moved @ x: %d, y: %d, theta: %.2f", pt.x, pt.y, dpObject->theta2);
		}
		drawObject();
	}

}
void animateDoublePendulum()
{

}

void BasicDrawPane::paintEvent(wxPaintEvent &evt)
{
	originCircle = new CircleObject(*this, x_o, y_o, 5.0, wxBLACK_BRUSH, 2, wxYELLOW);
	originLine = new LineObject(*this, x_o - 50, y_o, x_o + 50, y_o, 5, wxRED);
	auto [theta1, lengthBob1, radiusBob1, theta2, lengthBob2, radiusBob2] = dpObject->getInitial();
	auto [xBob1, yBob1, xBob2, yBob2] = dpObject->calcPositions();
	bob1Circle = new CircleObject(*this, xBob1, yBob1, radiusBob1, wxGREEN_BRUSH, 2, wxGREEN);
	bob1Line = new LineObject(*this, x_o, y_o, xBob1, yBob1, 2, wxBLACK);
	bob2Circle = new CircleObject(*this, xBob2, yBob2, radiusBob2, wxBLUE_BRUSH, 2, wxBLUE);
	bob2Line = new LineObject(*this, xBob1, yBob1, xBob2, yBob2, 2, wxBLACK);

	render();
}

void BasicDrawPane::render()
{
	// draw fixed items
	originCircle->draw();
	originLine->draw();
	bob1Circle->draw();
	bob1Line->draw();
	bob2Circle->draw();
	bob2Line->draw();
}

void BasicDrawPane::drawObject()
{
	auto [xBob1, yBob1, xBob2, yBob2] = dpObject->calcPositions();
	bob1Circle->update(xBob1, yBob1);
	bob1Line->update(x_o, y_o, xBob1, yBob1);
	bob2Circle->update(xBob2, yBob2);
	bob2Line->update(xBob1, yBob1, xBob2, yBob2);
	wxClientDC dc(this);
	dc.Clear();
	render();
}
