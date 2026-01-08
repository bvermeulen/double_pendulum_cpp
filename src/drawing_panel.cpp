#include <cstdio>
#include <time.h>
#include <tuple>
#include <wx/wx.h>
#include <gonio_funcs.h>
#include <doublependulum.h>
#include <drawing_panel.h>
#include <objects.h>
#define CLOCKS_PER_MS (CLOCKS_PER_SEC * 0.001)
#define FRAME_RATE 25
#define FRAME_RATE_MARGIN 10

wxDEFINE_EVENT(EVT_UPDATE_VALUES, wxCommandEvent);


DrawingPanel::DrawingPanel(wxFrame *parent) : wxPanel(parent)
{
	x_o = 0.0;
	y_o = 0.0;
	modelFactor = 25;
	radiusFactor = 2.0;
	originLineLength = 100;
	originSize = 10;
	timeCounter = 0;
	dragBob1Enabled = false;
	dragBob2Enabled = false;
	runEnabled = false;
	paintEventDone = false;
	tracerEnabled = false;
	deltaTime = 0.01; // 0.0001;
	dpObject = new DoublePendulum();

	Bind(wxEVT_PAINT, paintEvent, this);
	Bind(wxEVT_MOTION, mouseMoved, this);
	Bind(wxEVT_LEFT_DOWN, leftClick, this);
	Bind(wxEVT_RIGHT_DOWN, rightClick, this);
	Bind(wxEVT_SIZE, onSize, this);
}

std::tuple<double, double, double, double, double, double> DrawingPanel::fitToPanel(
	double x1, double y1,
	double x2, double y2,
	double r1, double r2
)
{
	x1 = x_o + x1 * modelFactor;
	y1 = y_o + y1 * modelFactor;
	x2 = x_o + x2 * modelFactor;
	y2 = y_o + y2 * modelFactor;
	r1 *= radiusFactor;
	r2 *= radiusFactor;
	return std::tuple(x1, y1, x2, y2, r1, r2);
}

void DrawingPanel::paintEvent(wxPaintEvent &event)
{
	originCircle = new CircleObject(this, x_o, y_o, originSize, wxYELLOW_BRUSH, 5, wxBLACK);
	originLine = new LineObject(this, x_o - originLineLength * 0.5, y_o, x_o + originLineLength * 0.5, y_o, 5, wxRED);
	auto [radiusBob1, radiusBob2] = dpObject->getRadiusSize();
	auto [xBob1, yBob1, xBob2, yBob2] = dpObject->getPositions();
	std::tie(xBob1, yBob1, xBob2, yBob2, radiusBob1, radiusBob2) = fitToPanel(
		xBob1, yBob1, xBob2, yBob2, radiusBob1, radiusBob2
	);
	bob1Circle = new CircleObject(this, xBob1, yBob1, radiusBob1, wxGREEN_BRUSH, 2, wxGREEN);
	bob1Line = new LineObject(this, x_o, y_o, xBob1, yBob1, 2, wxBLACK);
	bob2Circle = new CircleObject(this, xBob2, yBob2, radiusBob2, wxBLUE_BRUSH, 2, wxBLUE);
	bob2Line = new LineObject(this, xBob1, yBob1, xBob2, yBob2, 2, wxBLACK);
	tracerLine = new TracerObject(this, 1, wxBLACK);

	drawObjects();
	paintEventDone = true;
}

void DrawingPanel::onSize(wxSizeEvent &event)
{
	wxSize size = event.GetSize();
	printf("\nSize of drawingPane: %d, %d", size.x, size.y);
	x_o = size.x * 0.5;
	y_o = size.y * 0.5;
	if (paintEventDone)
	{
		updateObjects();
	}
	event.Skip();
}

std::tuple<float, float, float, float, float> DrawingPanel::getSettings()
{
	auto [_massBob1, _lengthBob1, _massBob2, _lengthBob2, _dampingFactor] = dpObject->getSettings();
	return {_massBob1, _lengthBob1, _massBob2, _lengthBob2, _dampingFactor};
}

void DrawingPanel::setSettings(float _massBob1, float _lengthBob1, float _massBob2, float _lengthBob2, float _dampingFactor)
{
	dpObject->setSettings(_massBob1, _lengthBob1, _massBob2, _lengthBob2, _dampingFactor);
	updateObjects();
}

double DrawingPanel::getTime()
{
	return _time;
}

void DrawingPanel::leftClick(wxMouseEvent &event)
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

void DrawingPanel::rightClick(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	dragBob1Enabled = false;
	dragBob2Enabled = false;
	runEnabled = false;
	dpObject -> clearThetaDotDoubleDot();
	printf("\nright click @ x: %d, y: %d, runabled: %d", pt.x, pt.y, runEnabled);
}

void DrawingPanel::mouseMoved(wxMouseEvent &event)
{
	if (dragBob1Enabled || dragBob2Enabled)
	{
		wxPoint pt = event.GetPosition();
		double x = (pt.x - x_o) / modelFactor;
		double y = (pt.y - y_o) / modelFactor;
		if (dragBob1Enabled)
		{
			dpObject->updateThetaBob1(x, y);
		}
		else
		{
			dpObject->updateThetaBob2(x, y);
		}
		updateObjects();
	}
}

void DrawingPanel::animateDoublePendulum()
{
	_time = clock() / CLOCKS_PER_MS;
	double _now = _time;
	newTimeCounter_1 = 0;
	newTimeCounter_2 = 0;
	newTimeCounter_3 = 0;
	while (runEnabled)
	{
		dpObject->calcThetaDotBoost(deltaTime);
		_time += deltaTime * 1000;
		timeCounter++;

		// wait loop for time to keep in pace
		while (_now < _time)
		{
			if (!runEnabled)
				break;
			_now = clock() / CLOCKS_PER_MS;
			wxYield();
		}

		// blit every FRAME_RATE msecs - 50 times per second for a FRAME_RATE of 25.
		if (timeCounter % int(FRAME_RATE / (1000 * deltaTime)) == 0 && timeCounter != newTimeCounter_1)
		{
			updateObjects();
			newTimeCounter_1 = timeCounter;
		}

		// every 1 second (1000 ms)
		if (timeCounter % int(1000 / (1000 * deltaTime)) == 0 && timeCounter != newTimeCounter_2)
		{
			updateValues();
			newTimeCounter_2 = timeCounter;
		}
	}
}

void DrawingPanel::controlAction(Control control)
{
	switch (control)
	{
		case START:
			runEnabled = true;
			tracerLine->clear();
			dpObject->clearThetaDotDoubleDot();
			animateDoublePendulum();
			break;
		case STOP:
			runEnabled = false;
			dpObject->clearThetaDotDoubleDot();
			break;
		case PAUSE:
			runEnabled = false;
			break;
		case RUN:
			runEnabled = true;
			animateDoublePendulum();
			break;
		case TRACE_ON:
			tracerEnabled = true;
			updateObjects();
			break;
		case TRACE_OFF:
			tracerEnabled = false;
			updateObjects();
			break;
		case TRACE_CLEAR:
			tracerLine->clear();
			updateObjects();
			break;
		case SWITCHCOLOR:
			auto [BrushColor1, Color1] = bob1Circle->getColors();
			auto [BrushColor2, Color2] = bob2Circle->getColors();
			bob1Circle->setColors(BrushColor2, Color2);
			bob2Circle->setColors(BrushColor1, Color1);
			updateObjects();
			break;
	}
}

void DrawingPanel::drawObjects()
{
	// draw fixed items
	if (tracerEnabled)
	tracerLine->draw();
	bob1Circle->draw();
	bob1Line->draw();
	originCircle->draw();
	originLine->draw();
	bob2Circle->draw();
	bob2Line->draw();
}

void DrawingPanel::updateObjects()
{
	auto [xBob1, yBob1, xBob2, yBob2] = dpObject->getPositions();
	auto [radiusBob1, radiusBob2] = dpObject->getRadiusSize();
	std::tie(xBob1, yBob1, xBob2, yBob2, radiusBob1, radiusBob2) = fitToPanel(
		xBob1, yBob1, xBob2, yBob2, radiusBob1, radiusBob2
	);
	originCircle->update(x_o, y_o, originSize);
	originLine->update(x_o - originLineLength * 0.5, y_o, x_o + originLineLength * 0.5, y_o);
	bob1Circle->update(xBob1, yBob1, radiusBob1);
	bob1Line->update(x_o, y_o, xBob1, yBob1);
	bob2Circle->update(xBob2, yBob2, radiusBob2);
	bob2Line->update(xBob1, yBob1, xBob2, yBob2);
	if (tracerEnabled && timeCounter % int(50 / (1000 * deltaTime)) == 0 && timeCounter != newTimeCounter_3)
	{
		tracerLine->update(xBob2, yBob2);
		newTimeCounter_3 = timeCounter;
		unsigned int size = tracerLine->getSize();
		if (size % 100 == 0)
		printf("\ntracer vector size: %u", size);
	}
	wxClientDC dc(this);
	dc.Clear();
	drawObjects();
}

void DrawingPanel::updateValues()
{
	wxCommandEvent customEvent(EVT_UPDATE_VALUES, GetId());
	customEvent.SetString(std::to_string(timeCounter));
	wxPostEvent(this, customEvent);
}

