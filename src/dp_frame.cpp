#include <wx/sizer.h>
#include <wx/wx.h>
#include <dp_frame.h>
#include <drawing_panel.h>

BEGIN_EVENT_TABLE(DoublePendulumFrame, wxFrame)
EVT_CLOSE(DoublePendulumFrame::onClose)
EVT_BUTTON(BUTTON_START, DoublePendulumFrame::onStart)
EVT_BUTTON(BUTTON_PAUSE, DoublePendulumFrame::onPause)
EVT_BUTTON(BUTTON_CLEARTRACE, DoublePendulumFrame::onClearTrace)
EVT_BUTTON(BUTTON_TOGGLETRACE, DoublePendulumFrame::onToggleTrace)
EVT_BUTTON(BUTTON_SWITCHCOLOR, DoublePendulumFrame::onSwitchColor)
EVT_SLIDER(MASSBOB_1, DoublePendulumFrame::onMassBob_1)
EVT_SLIDER(MASSBOB_2, DoublePendulumFrame::onMassBob_2)
EVT_SLIDER(LENGTHBOB_1, DoublePendulumFrame::onLengthBob_1)
EVT_SLIDER(LENGTHBOB_2, DoublePendulumFrame::onLengthBob_2)
END_EVENT_TABLE()


DoublePendulumFrame::DoublePendulumFrame() : wxFrame((wxFrame *)NULL, -1, wxT("Double Pendulum"), wxPoint(0, 0), wxSize(800, 620))
{
	sliderFactor = 10.0;
	sliderSize = wxSize(160, 30);
	startEnabled = false;
	pauseEnabled = false;
	traceEnabled = false;
	initUI();
}

DoublePendulumFrame::~DoublePendulumFrame()
{
}

void DoublePendulumFrame::initUI()
{
	drawingPanel = new DrawingPanel(this);
	auto [massBob1, lengthBob1, massBob2, lengthBob2, dampingFactor] = drawingPanel->getSettings();
	massBob1 *= sliderFactor;
	massBob2 *= sliderFactor;
	lengthBob1 *= sliderFactor;
	lengthBob2 *= sliderFactor;
	dampingFactor *= sliderFactor;

	wxMenuBar *menuBarPointer = new wxMenuBar();
	wxMenu *menuFilePointer = new wxMenu();
	menuFilePointer->Append(wxID_EXIT, _T("Quit"));
	menuBarPointer->Append(menuFilePointer, _T("File"));
	SetMenuBar(menuBarPointer);
	Bind(wxEVT_MENU, &DoublePendulumFrame::onExit, this, wxID_EXIT);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *mainBox = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *graphBox = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *controlBox = new wxBoxSizer(wxHORIZONTAL);

	// add items in mainBox
	wxPanel *settingsPanel = new wxPanel(this);
	settingsPanel->SetMinSize(wxSize(170, -1));
	settingsPanel->SetMaxSize(wxSize(200, -1));
	wxBoxSizer *settingsBox = new wxBoxSizer(wxVERTICAL);
	wxStaticText *label_1 = new wxStaticText(settingsPanel, wxID_ANY, wxT("Mass Bob 1 (*0.1)"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	sMassBob_1 = new wxSlider(settingsPanel, MASSBOB_1, massBob1, 0, 100, wxDefaultPosition, sliderSize, wxSL_LABELS);
	sMassBob_1->SetPageSize(5);
	settingsBox->Add(label_1, 0, wxLEFT | wxBOTTOM, 5);
	settingsBox->Add(sMassBob_1, 0, wxLEFT, 5);
	settingsBox->AddSpacer(30);
	wxStaticText *label_2 = new wxStaticText(settingsPanel, wxID_ANY, wxT("Mass Bob 2 (*0.1)"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	sMassBob_2 = new wxSlider(settingsPanel, MASSBOB_2, massBob2, 0, 100, wxDefaultPosition, sliderSize, wxSL_LABELS);
	sMassBob_2->SetPageSize(5);
	settingsBox->Add(label_2, 0, wxLEFT | wxBOTTOM, 5);
	settingsBox->Add(sMassBob_2, 0, wxLEFT, 5);
	settingsBox->AddSpacer(30);
	wxStaticText *label_3 = new wxStaticText(settingsPanel, wxID_ANY, wxT("Length Bob 1 (*0.1)"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	sLengthBob_1 = new wxSlider(settingsPanel, LENGTHBOB_1, lengthBob1, 0, 100, wxDefaultPosition, sliderSize, wxSL_LABELS);
	sLengthBob_1->SetPageSize(5);
	settingsBox->Add(label_3, 0, wxLEFT | wxBOTTOM, 5);
	settingsBox->Add(sLengthBob_1, 0, wxLEFT, 5);
	settingsBox->AddSpacer(30);
	wxStaticText *label_4 = new wxStaticText(settingsPanel, wxID_ANY, wxT("Length Bob 2 (*0.1)"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	sLengthBob_2 = new wxSlider(settingsPanel, LENGTHBOB_2, lengthBob2, 0, 100, wxDefaultPosition, sliderSize, wxSL_LABELS);
	sLengthBob_2->SetPageSize(5);
	settingsBox->Add(label_4, 0, wxLEFT | wxBOTTOM, 5);
	settingsBox->Add(sLengthBob_2, 0, wxLEFT, 5);
	settingsBox->AddSpacer(30);

	settingsPanel->SetSizer(settingsBox);

	drawingPanel->SetMinSize(wxSize(600, 400));
	mainBox->Add(settingsPanel, true, wxTOP | wxLEFT, 10);
	mainBox->AddSpacer(10);
	mainBox->Add(drawingPanel, true, wxEXPAND | wxTOP | wxRIGHT, 10);

	// add items in graphBox
	wxPanel *theta1Pane = new wxPanel(this);
	theta1Pane->SetMinSize(wxSize(400, 100));
	wxPanel *theta2Pane = new wxPanel(this);
	theta2Pane->SetMinSize(wxSize(400, 100));
	graphBox->Add(theta1Pane, true, wxEXPAND | wxTOP | wxLEFT, 10);
	graphBox->AddSpacer(10);
	graphBox->Add(theta2Pane, true, wxEXPAND | wxTOP | wxRIGHT, 10);

	// add items in controlBox
	startBtn = new wxButton(this, BUTTON_START, wxT("Start"));
	pauseBtn = new wxButton(this, BUTTON_PAUSE, wxT("Pause"));
	toggleTraceBtn = new wxButton(this, BUTTON_TOGGLETRACE, wxT("Trace on"));
	wxButton *clearTraceBtn = new wxButton(this, BUTTON_CLEARTRACE, wxT("Clear trace"));
	wxButton *switchColorBtn = new wxButton(this, BUTTON_SWITCHCOLOR, wxT("Switch color"));
	controlBox->Add(startBtn, true, wxTOP | wxBOTTOM | wxLEFT, 10);
	controlBox->Add(pauseBtn, true, wxTOP | wxBOTTOM, 10);
	controlBox->Add(toggleTraceBtn, true, wxTOP | wxBOTTOM, 10);
	controlBox->Add(clearTraceBtn, true, wxTOP | wxBOTTOM, 10);
	controlBox->Add(switchColorBtn, true, wxTOP | wxBOTTOM, 10);

	// collate in the vertical box
	vbox->Add(mainBox, wxEXPAND);
	vbox->Add(graphBox);
	vbox->Add(controlBox);

	this->SetSizer(vbox);
	this->Show();
	this->Centre();
}

void DoublePendulumFrame::onExit(wxCommandEvent &event)
{
	printf("onExit: I want to close thie app!!!!");
	this->Destroy();
}

void DoublePendulumFrame::onClose(wxCloseEvent &event)
{
	printf("onExit: I want to close thie app!!!!");
	this->Destroy();
}

void DoublePendulumFrame::onStart(wxCommandEvent &event)
{
	startEnabled = !startEnabled;
	if (startEnabled)
	{
		startBtn->SetLabelText(wxT("Stop"));
		pauseEnabled = false;
		pauseBtn->SetLabelText(wxT("Pause"));
		drawingPanel->controlAction(drawingPanel->START);
	}
	else
	{
		startBtn->SetLabelText(wxT("Start"));
		drawingPanel->controlAction(drawingPanel->STOP);
	}
}

void DoublePendulumFrame::onPause(wxCommandEvent &event)
{
	pauseEnabled = !pauseEnabled;
	if (pauseEnabled)
	{
		pauseBtn->SetLabelText(wxT("Run"));
		drawingPanel->controlAction(drawingPanel->PAUSE);
	}
	else
	{
		pauseBtn->SetLabelText(wxT("Pause"));
		drawingPanel->controlAction(drawingPanel->RUN);
	}
}

void DoublePendulumFrame::onToggleTrace(wxCommandEvent &event)
{
	traceEnabled = !traceEnabled;
	if (traceEnabled)
	{
		toggleTraceBtn->SetLabelText(wxT("Trace off"));
		drawingPanel->controlAction(drawingPanel->TRACE_ON);
	}
	else
	{
		toggleTraceBtn->SetLabelText(wxT("Trace on"));
		drawingPanel->controlAction(drawingPanel->TRACE_OFF);
	}
}

void DoublePendulumFrame::onClearTrace(wxCommandEvent &event)
{
	drawingPanel->controlAction(drawingPanel->TRACE_CLEAR);
}

void DoublePendulumFrame::onSwitchColor(wxCommandEvent &event)
{
	drawingPanel->controlAction(drawingPanel->SWITCHCOLOR);
}

void DoublePendulumFrame::onMassBob_1(wxCommandEvent &event)
{
	drawingPanel->setSettings(sMassBob_1->GetValue() / sliderFactor, -1, -1, -1, -1);
}

void DoublePendulumFrame::onMassBob_2(wxCommandEvent &event)
{
	drawingPanel->setSettings(-1, -1, sMassBob_2->GetValue() / sliderFactor, -1, -1);
}

void DoublePendulumFrame::onLengthBob_1(wxCommandEvent &event)
{
	drawingPanel->setSettings(-1, sLengthBob_1->GetValue() / sliderFactor, -1, -1, -1);
}

void DoublePendulumFrame::onLengthBob_2(wxCommandEvent &event)
{
	drawingPanel->setSettings(-1, -1, -1, sLengthBob_2->GetValue() / sliderFactor, - 1);
}