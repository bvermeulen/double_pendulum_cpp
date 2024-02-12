#include <cstdio>
#include <time.h>
//#include <tuple>
#include <wx/wx.h>
#include <gonio_funcs.h>
#include <doublependulum.h>
#include <drawing_pane.h>
#include <objects.h>
#define CLOCKS_PER_MS (CLOCKS_PER_SEC * 0.001)

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
	x_o = 520.0;
	y_o = 360.0;
	blitCount = 0;
	dragBob1Enabled = false;
	dragBob2Enabled = false;
	runEnabled = false;
	dpObject = new DoublePendulum(x_o, y_o);
}

void BasicDrawPane::leftClick(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	printf("\nleft click @ x: %d, y: %d", pt.x, pt.y);
	if (bob1Circle->mouseHover(pt.x, pt.y))
		dragBob1Enabled = true;
	if (bob2Circle->mouseHover(pt.x, pt.y))
		dragBob2Enabled = true;
	if (originCircle->mouseHover(pt.x, pt.y))
		{
			runEnabled = true;
			animateDoublePendulum();
		}
}

void BasicDrawPane::rightClick(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	dragBob1Enabled = false;
	dragBob2Enabled = false;
	runEnabled= false;
	printf("\nright click @ x: %d, y: %d, runabled: %d", pt.x, pt.y, runEnabled);
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
void BasicDrawPane::animateDoublePendulum()
{
	float deltaTime = 0.0001;
	bool blit = false;
	float _time = clock() / CLOCKS_PER_MS;
	float _now = clock() / CLOCKS_PER_MS;
	while (runEnabled)
	{
		dpObject->calcThetaDoubleDot();
		dpObject->calcThetaDot(deltaTime);

		// wait loop for time to keep in pace
		_time += deltaTime * 1000;
		while (_now < _time)
		{
			if (!runEnabled) break;
			_now = clock() / CLOCKS_PER_MS;
			// blit every 25 msecs - 50 times per second
			if (((int)_now % 25 < 10)) blit = true;
			wxYield();
		}
		if (blit)
		{
			drawObject();
			blitCount++;
			blit = false;
		}

	}
}

void BasicDrawPane::paintEvent(wxPaintEvent &evt)
{
	originCircle = new CircleObject(*this, x_o, y_o, 8.0, wxBLACK_BRUSH, 5, wxYELLOW);
	originLine = new LineObject(*this, x_o - 50, y_o, x_o + 50, y_o, 5, wxRED);
	auto [theta1, lengthBob1, radiusBob1, theta2, lengthBob2, radiusBob2] = dpObject->getInitial();
	auto [xBob1, yBob1, xBob2, yBob2] = dpObject->calcPositions();
	bob1Circle = new CircleObject(*this, xBob1, yBob1, radiusBob1, wxGREEN_BRUSH, 2, wxGREEN);
	bob1Line = new LineObject(*this, x_o, y_o, xBob1, yBob1, 2, wxBLACK);
	bob2Circle = new CircleObject(*this, xBob2, yBob2, radiusBob2, wxBLUE_BRUSH, 2, wxBLUE);
	bob2Line = new LineObject(*this, xBob1, yBob1, xBob2, yBob2, 2, wxBLACK);
	tracerLine = new TracerObject(*this, 1, wxBLACK);

	render();
}

void BasicDrawPane::render()
{
	// draw fixed items
	tracerLine->draw();
	bob1Circle->draw();
	bob1Line->draw();
	originCircle->draw();
	originLine->draw();
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
	if (blitCount % 2 == 0) tracerLine->update(xBob2, yBob2);
	wxClientDC dc(this);
	dc.Clear();
	render();
}
