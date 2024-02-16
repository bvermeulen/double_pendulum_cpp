#include <cstdio>
#include <time.h>
#include <tuple>
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
EVT_SIZE(DrawingPane::onSize)
END_EVENT_TABLE()

DrawingPane::DrawingPane(wxFrame *parent) : wxPanel(parent)
{
	originLineLength = 100;
	originSize = 10;
	blitCount = 0;
	dragBob1Enabled = false;
	dragBob2Enabled = false;
	runEnabled = false;
	paintEventDone = false;
	tracerEnabled = false;
	dpObject = new DoublePendulum();
}

void DrawingPane::paintEvent(wxPaintEvent &evt)
{
	dpObject->updateOrigin(x_o, y_o);
	originCircle = new CircleObject(*this, x_o, y_o, originSize, wxYELLOW_BRUSH, 5, wxBLACK);
	originLine = new LineObject(*this, x_o - originLineLength * 0.5, y_o, x_o + originLineLength * 0.5, y_o, 5, wxRED);
	auto [radiusBob1, radiusBob2] = dpObject->getRadiusSize();
	auto [xBob1, yBob1, xBob2, yBob2] = dpObject->getPositions();
	bob1Circle = new CircleObject(*this, xBob1, yBob1, radiusBob1, wxGREEN_BRUSH, 2, wxGREEN);
	bob1Line = new LineObject(*this, x_o, y_o, xBob1, yBob1, 2, wxBLACK);
	bob2Circle = new CircleObject(*this, xBob2, yBob2, radiusBob2, wxBLUE_BRUSH, 2, wxBLUE);
	bob2Line = new LineObject(*this, xBob1, yBob1, xBob2, yBob2, 2, wxBLACK);
	tracerLine = new TracerObject(*this, 1, wxBLACK);

	drawObjects();
	paintEventDone = true;
}

void DrawingPane::onSize(wxSizeEvent &event)
{
	wxSize size = event.GetSize();
	printf("\nSize of drawingPane: %d, %d", size.x, size.y);
	x_o = size.x * 0.5;
	y_o = size.y * 0.5;
	if (paintEventDone)
	{
		dpObject->updateOrigin(x_o, y_o);
		updateObjects();
	}
	event.Skip();
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

std::tuple<float, float, float, float, float> DrawingPane::getSettings()
{
	auto [_massBob1, _lengthBob1, _massBob2, _lengthBob2, _dampingFactor] = dpObject->getSettings();
	return { _massBob1, _lengthBob1, _massBob2, _lengthBob2, _dampingFactor };
}

void DrawingPane::setSettings(float _massBob1, float _lengthBob1, float _massBob2, float _lengthBob2, float _dampingFactor)
{
	//if (!paintEventDone) return;
	dpObject->setSettings(_massBob1, _lengthBob1, _massBob2, _lengthBob2, _dampingFactor);
	updateObjects();
}

void DrawingPane::rightClick(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	dragBob1Enabled = false;
	dragBob2Enabled = false;
	runEnabled = false;
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
		updateObjects();
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
			if (!runEnabled)
				break;
			_now = clock() / CLOCKS_PER_MS;
			// blit every 25 msecs - 50 times per second
			if (((int)_now % 25 < 10))
				blit = true;
			wxYield();
		}
		if (blit)
		{
			updateObjects();
			blitCount++;
			blit = false;
		}
	}
}

void DrawingPane::controlAction(Control control)
{
	switch (control)
	{
	case START:
		runEnabled = true;
		animateDoublePendulum();
		break;
	case STOP:
		runEnabled = false;
		break;
	case TOGGLETRACE:
		tracerEnabled = !tracerEnabled;
		break;
	case CLEARTRACE:
		tracerLine->clear();
		break;
	case SWITCHCOLOR:
		printf("\nNot yet implemented ...");
		break;
	}
}

void DrawingPane::drawObjects()
{
	// draw fixed items
	if (tracerEnabled) tracerLine->draw();
	bob1Circle->draw();
	bob1Line->draw();
	originCircle->draw();
	originLine->draw();
	bob2Circle->draw();
	bob2Line->draw();
}

void DrawingPane::updateObjects()
{
	auto [xBob1, yBob1, xBob2, yBob2] = dpObject->getPositions();
	auto [radiusBob1, radiusBob2] = dpObject->getRadiusSize();
	originCircle->update(x_o, y_o, originSize);
	originLine->update(x_o - originLineLength * 0.5, y_o, x_o + originLineLength * 0.5, y_o);
	bob1Circle->update(xBob1, yBob1, radiusBob1);
	bob1Line->update(x_o, y_o, xBob1, yBob1);
	bob2Circle->update(xBob2, yBob2, radiusBob2);
	bob2Line->update(xBob1, yBob1, xBob2, yBob2);
	if (blitCount % 2 == 0)
		tracerLine->update(xBob2, yBob2);
	wxClientDC dc(this);
	dc.Clear();
	drawObjects();
}
