#include <cstdio>
#include <tuple>
#include <wx/wx.h>
#include <config.h>
#include <gonio_funcs.h>
#include <doublependulum.h>
#include <main_panel.h>
#include <objects.h>

wxDEFINE_EVENT(EVT_UPDATE_VALUES, wxCommandEvent);

MainPanel::MainPanel(wxFrame *parent, DoublePendulum &dpObjectRef) : wxPanel(parent), dpObject(dpObjectRef)
{
	x_o = 0.0;
	y_o = 0.0;
	modelFactor = 25;
	radiusFactor = 2.0;
	originLineLength = 100;
	originSize = 10;
	dragBob1Enabled = false;
	dragBob2Enabled = false;
	runEnabled = false;
	paintEventDone = false;
	tracerEnabled = false;
	deltaTime = DELTA_TIME;

	Bind(wxEVT_PAINT, paintEvent, this);
	Bind(wxEVT_MOTION, mouseMoved, this);
	Bind(wxEVT_LEFT_DOWN, leftClick, this);
	Bind(wxEVT_RIGHT_DOWN, rightClick, this);
	Bind(wxEVT_SIZE, onSize, this);

	updateMonitorValues();
}

MainPanel::~MainPanel()
{
	delete originCircle;
	delete originLine;
	delete bob1Circle;
	delete bob1Line;
	delete bob2Circle;
	delete bob2Line;
	delete tracerLine;
}

std::tuple<double, double, double, double, double, double> MainPanel::fitToPanel(
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

void MainPanel::paintEvent(wxPaintEvent &event)
{
	originCircle = new CircleObject(this, x_o, y_o, originSize, wxYELLOW_BRUSH, 5, wxBLACK);
	originLine = new LineObject(this, x_o - originLineLength * 0.5, y_o, x_o + originLineLength * 0.5, y_o, 5, wxRED);
	auto [radiusBob1, radiusBob2] = dpObject.getRadiusSize();
	auto [xBob1, yBob1, xBob2, yBob2] = dpObject.getPositions();
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

void MainPanel::onSize(wxSizeEvent &event)
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

void MainPanel::leftClick(wxMouseEvent &event)
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

void MainPanel::rightClick(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	dragBob1Enabled = false;
	dragBob2Enabled = false;
	runEnabled = false;
	dpObject.clearThetaDotDoubleDot();
	printf("\nright click @ x: %d, y: %d, runabled: %d", pt.x, pt.y, runEnabled);
}

void MainPanel::mouseMoved(wxMouseEvent &event)
{
	if (dragBob1Enabled || dragBob2Enabled)
	{
		wxPoint pt = event.GetPosition();
		double x = (pt.x - x_o) / modelFactor;
		double y = (pt.y - y_o) / modelFactor;
		if (dragBob1Enabled)
		{
			dpObject.updateThetaBob1(x, y);
		}
		else
		{
			dpObject.updateThetaBob2(x, y);
		}
		updateObjects();
		updateMonitorValues();
	}
}

void MainPanel::animateDoublePendulum()
{
	newClockTime_1 = 0;
	newClockTime_2 = 0;
	newClockTime_3 = 0;
	AnimationTime += int(deltaTime * 1e6);
	while (runEnabled)
	{
		clockTimeMicro = stopWatch.TimeInMicro();
		if  (clockTimeMicro > AnimationTime)
		{
			dpObject.calcThetaDotBoost(deltaTime);
			AnimationTime += int(deltaTime * 1e6);
		}

		clockTimeMilli = int(clockTimeMicro.GetValue() * 0.001);

		// blit every FRAME_RATE msecs - 50 times per second for a FRAME_RATE of 25.
		if (clockTimeMilli % FRAME_RATE == 0 && clockTimeMilli != newClockTime_1)
		{
			updateObjects();
			newClockTime_1 = clockTimeMilli;
		}

		// every 0.1 second (100 ms)
		if (clockTimeMilli % FRAME_RATE_MONITOR == 0 && clockTimeMilli != newClockTime_2)
		{
			updateMonitorValues();
			newClockTime_2 = clockTimeMilli;
		}
		if (!runEnabled)
			break;
		wxYield();
	}
}

void MainPanel::controlAction(Control control)
{
	switch (control)
	{
		case START:
			runEnabled = true;
			tracerLine->clear();
			dpObject.clearThetaDotDoubleDot();
			stopWatch.Start();
			AnimationTime = 0.0;
			animateDoublePendulum();
			break;
		case STOP:
			runEnabled = false;
			stopWatch.Pause();
			dpObject.clearThetaDotDoubleDot();
			break;
		case PAUSE:
			stopWatch.Pause();
			runEnabled = false;
			break;
		case RUN:
			runEnabled = true;
			stopWatch.Resume();
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
		{
			auto [BrushColor1, Color1] = bob1Circle->getColors();
			auto [BrushColor2, Color2] = bob2Circle->getColors();
			bob1Circle->setColors(BrushColor2, Color2);
			bob2Circle->setColors(BrushColor1, Color1);
			updateObjects();
			break;
		}
		case UPDATE_PANEL:
			updateObjects();
			break;
	}
}

void MainPanel::drawObjects()
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

void MainPanel::updateObjects()
{
	auto [xBob1, yBob1, xBob2, yBob2] = dpObject.getPositions();
	auto [radiusBob1, radiusBob2] = dpObject.getRadiusSize();
	std::tie(xBob1, yBob1, xBob2, yBob2, radiusBob1, radiusBob2) = fitToPanel(
		xBob1, yBob1, xBob2, yBob2, radiusBob1, radiusBob2
	);
	originCircle->update(x_o, y_o, originSize);
	originLine->update(x_o - originLineLength * 0.5, y_o, x_o + originLineLength * 0.5, y_o);
	bob1Circle->update(xBob1, yBob1, radiusBob1);
	bob1Line->update(x_o, y_o, xBob1, yBob1);
	bob2Circle->update(xBob2, yBob2, radiusBob2);
	bob2Line->update(xBob1, yBob1, xBob2, yBob2);
	if (tracerEnabled && clockTimeMilli % TRACER_UPDATE == 0 && clockTimeMilli != newClockTime_3)
	{
		tracerLine->update(xBob2, yBob2);
		newClockTime_3 = clockTimeMicro.GetValue();
		unsigned int size = tracerLine->getSize();
		if (size % 100 == 0)
		printf("\ntracer vector size: %u", size);
	}
	wxClientDC dc(this);
	dc.Clear();
	drawObjects();
}

void MainPanel::updateMonitorValues()
{
	wxCommandEvent customEvent(EVT_UPDATE_VALUES, GetId());
	wxPostEvent(this, customEvent);
}

std::tuple<wxColor, wxColor> MainPanel::getBobColorsTuple()
{
	const wxColor* bob1Color = std::get<1>(bob1Circle->getColors());
	const wxColor* bob2Color = std::get<1>(bob2Circle->getColors());
	auto bobColorsTuple = std::make_tuple(wxColor(*bob1Color), wxColor(*bob2Color));
	return bobColorsTuple;
}