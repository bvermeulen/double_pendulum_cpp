#include <cstdio>
#include <time.h>
#include <wx/wx.h>
#include <gonio_funcs.h>
#include <doublependulum.h>
#include <drawing_pane.h>
#include <objects.h>
#define CLOCKS_PER_MS (CLOCKS_PER_SEC * 0.001)

// catch paint events
BEGIN_EVENT_TABLE(DrawingPane, wxPanel)
EVT_PAINT(DrawingPane::paintEvent)
EVT_MOTION(DrawingPane::mouseMoved)
// EVT_LEFT_DOWN(DrawingPane::mouseDown)
// EVT_LEFT_UP(DrawingPane::mouseReleased)
EVT_LEFT_DOWN(DrawingPane::leftClick)
EVT_RIGHT_DOWN(DrawingPane::rightClick)
// EVT_LEAVE_WINDOW(DrawingPane::mouseLeftWindow)
// EVT_KEY_DOWN(DrawingPane::keyPressed)
// EVT_KEY_UP(DrawingPane::keyReleased)
// EVT_MOUSEWHEEL(DrawingPane::mouseWheelMoved)
END_EVENT_TABLE()

DrawingPane::DrawingPane(wxFrame *parent) : wxPanel(parent)
{
	x_o = 250.0;
	y_o = 200.0;
	blitCount = 0;
	dragBob1Enabled = false;
	dragBob2Enabled = false;
	runEnabled = false;
	dpObject = new DoublePendulum(x_o, y_o);
	//SetDoubleBuffered(true);
}

void DrawingPane::leftClick(wxMouseEvent &event)
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

void DrawingPane::rightClick(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	dragBob1Enabled = false;
	dragBob2Enabled = false;
	runEnabled= false;
	printf("\nright click @ x: %d, y: %d, runabled: %d", pt.x, pt.y, runEnabled);
}

void DrawingPane::mouseMoved(wxMouseEvent &event)
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
void DrawingPane::animateDoublePendulum()
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

void DrawingPane::controlAction(Control control)
{
	switch(control)
	{
		case START:
			runEnabled = true;
			animateDoublePendulum();
			break;
		case STOP:
			runEnabled = false;
			break;
		case TOGGLETRACE:
			printf("\nNot yet implemented ...");
			break;
		case CLEARTRACE:
			printf("\nNot yet implemented ...");
			break;
		case SWITCHCOLOR:
			printf("\nNot yet implemented ...");
			break;
	}
}


void DrawingPane::paintEvent(wxPaintEvent &evt)
{
	originCircle = new CircleObject(*this, x_o, y_o, 10.0, wxYELLOW_BRUSH, 5, wxBLACK);
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

void DrawingPane::render()
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

void DrawingPane::drawObject()
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
