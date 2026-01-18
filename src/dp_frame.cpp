#include <format>
#include <wx/sizer.h>
#include <wx/wx.h>
#include <config.h>
#include <dp_frame.h>
#include <main_panel.h>
#include <monitor_panels.h>
#include <doublependulum.h>
#include <gonio_funcs.h>


DoublePendulumFrame::DoublePendulumFrame() : wxFrame((wxFrame *)NULL, -1, wxT("Double Pendulum"), wxPoint(0, 0), wxSize(APP_WIDTH, APP_HEIGHT))
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
	delete dpObject;
	delete mainPanel;
}

void DoublePendulumFrame::initUI()
{
	
	dpObject = new DoublePendulum();
	auto [massBob1, lengthBob1, massBob2, lengthBob2, dampingFactor] = dpObject->getSettings();
	mainPanel = new MainPanel(this, *dpObject);
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
	
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *mainBox = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *graphBox = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *controlBox = new wxBoxSizer(wxHORIZONTAL);
	
	// add items in mainBox
	wxPanel *settingsPanel = new wxPanel(this);
	settingsPanel->SetMinSize(wxSize(SETTINGS_MIN_SIZE, -1));
	settingsPanel->SetMaxSize(wxSize(SETTINGS_MAX_SIZE, -1));
	wxBoxSizer *settingsBox = new wxBoxSizer(wxVERTICAL);
	wxStaticText *label_1 = new wxStaticText(settingsPanel, wxID_ANY, wxT("Mass Bob 1 (*0.1)"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	sMassBob_1 = new wxSlider(settingsPanel, wxID_ANY, massBob1, 0, 100, wxDefaultPosition, sliderSize, wxSL_LABELS);
	sMassBob_1->SetPageSize(5);
	settingsBox->Add(label_1, 0, wxLEFT | wxBOTTOM, 5);
	settingsBox->Add(sMassBob_1, 0, wxLEFT, 5);
	settingsBox->AddSpacer(30);
	wxStaticText *label_2 = new wxStaticText(settingsPanel, wxID_ANY, wxT("Mass Bob 2 (*0.1)"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	sMassBob_2 = new wxSlider(settingsPanel, wxID_ANY, massBob2, 0, 100, wxDefaultPosition, sliderSize, wxSL_LABELS);
	sMassBob_2->SetPageSize(5);
	settingsBox->Add(label_2, 0, wxLEFT | wxBOTTOM, 5);
	settingsBox->Add(sMassBob_2, 0, wxLEFT, 5);
	settingsBox->AddSpacer(30);
	wxStaticText *label_3 = new wxStaticText(settingsPanel, wxID_ANY, wxT("Length Bob 1 (*0.1)"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	sLengthBob_1 = new wxSlider(settingsPanel, wxID_ANY, lengthBob1, 0, 100, wxDefaultPosition, sliderSize, wxSL_LABELS);
	sLengthBob_1->SetPageSize(5);
	settingsBox->Add(label_3, 0, wxLEFT | wxBOTTOM, 5);
	settingsBox->Add(sLengthBob_1, 0, wxLEFT, 5);
	settingsBox->AddSpacer(30);
	wxStaticText *label_4 = new wxStaticText(settingsPanel, wxID_ANY, wxT("Length Bob 2 (*0.1)"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	sLengthBob_2 = new wxSlider(settingsPanel, wxID_ANY, lengthBob2, 0, 100, wxDefaultPosition, sliderSize, wxSL_LABELS);
	sLengthBob_2->SetPageSize(5);
	settingsBox->Add(label_4, 0, wxLEFT | wxBOTTOM, 5);
	settingsBox->Add(sLengthBob_2, 0, wxLEFT, 5);
	settingsBox->AddSpacer(30);

	wxBoxSizer *theta1Box = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *theta1Label = new wxStaticText(settingsPanel, wxID_ANY, wxT("theta1: "), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	theta1ValueBox = new wxTextCtrl( settingsPanel, wxID_ANY, wxT(" "), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	theta1Box->Add(theta1Label, 0, wxLEFT, 5);	
	theta1Box->Add(theta1ValueBox, 0, wxLEFT, 5);
	settingsBox->Add(theta1Box);
	settingsBox->AddSpacer(5);
	wxBoxSizer *theta2Box = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *theta2Label = new wxStaticText(settingsPanel, wxID_ANY, wxT("theta2: "), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	theta2ValueBox = new wxTextCtrl(settingsPanel, wxID_ANY, wxT(" "), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	theta2Box->Add(theta2Label, 0, wxLEFT, 5);	
	theta2Box->Add(theta2ValueBox, 0, wxLEFT, 5);
	settingsBox->Add(theta2Box);
	settingsBox->AddSpacer(10);
	
	timeLabel = new wxStaticText(settingsPanel, wxID_ANY, wxT("time: 0.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	settingsBox->Add(timeLabel, 0, wxLEFT | wxBOTTOM, 5);
	settingsBox->AddSpacer(10);
	settingsPanel->SetSizer(settingsBox);
	
	mainBox->Add(settingsPanel, true, wxTOP | wxLEFT, 10);
	mainBox->AddSpacer(10);
	mainPanel->SetMinSize(wxSize(MAINPANEL_MIN_SIZE_WIDTH, MAINPANEL_MIN_SIZE_HEIGHT));
	mainBox->Add(mainPanel, true, wxEXPAND | wxTOP | wxRIGHT, 10);
	
	// add items in graphBox
	monitorPanelTheta1 = new MonitorPanelTheta1(this, 25);
	monitorPanelTheta1->SetMinSize(wxSize(MONITOR_MIN_SIZE_WIDTH, MONITOR_MIN_SIZEHEIGHT));
	graphBox->Add(monitorPanelTheta1, true, wxLEFT | wxTOP , 10);
	graphBox->AddSpacer(10);
	
	// add items in controlBox
	startBtn = new wxButton(this, wxID_ANY, wxT("Start"));
	pauseBtn = new wxButton(this, wxID_ANY, wxT("Pause"));
	toggleTraceBtn = new wxButton(this, wxID_ANY, wxT("Trace on"));
	wxButton *clearTraceBtn = new wxButton(this, wxID_ANY, wxT("Clear trace"));
	wxButton *switchColorBtn = new wxButton(this, wxID_ANY, wxT("Switch color"));
	controlBox->Add(startBtn, true, wxTOP | wxBOTTOM | wxLEFT, 10);
	controlBox->Add(pauseBtn, true, wxTOP | wxBOTTOM, 10);
	controlBox->Add(toggleTraceBtn, true, wxTOP | wxBOTTOM, 10);
	controlBox->Add(clearTraceBtn, true, wxTOP | wxBOTTOM, 10);
	controlBox->Add(switchColorBtn, true, wxTOP | wxBOTTOM, 10);
	
	// collate in the vertical box
	vbox->Add(mainBox, wxEXPAND);
	vbox->Add(graphBox);
	vbox->Add(controlBox);
	
	SetSizer(vbox);
	Show();
	Centre();
	
	Bind(wxEVT_MENU, onExit, this);
	Bind(wxEVT_CLOSE_WINDOW, onClose, this);
	startBtn->Bind(wxEVT_BUTTON, onStart, this);
	pauseBtn->Bind(wxEVT_BUTTON, onPause, this);
	clearTraceBtn->Bind(wxEVT_BUTTON, onClearTrace, this);
	toggleTraceBtn->Bind(wxEVT_BUTTON, onToggleTrace, this);
	switchColorBtn->Bind(wxEVT_BUTTON, onSwitchColor, this);
	sMassBob_1->Bind(wxEVT_SLIDER, onMassBob_1, this);
	sMassBob_2->Bind(wxEVT_SLIDER, onMassBob_2, this);
	sLengthBob_1->Bind(wxEVT_SLIDER, onLengthBob_1, this);
	sLengthBob_2->Bind(wxEVT_SLIDER, onLengthBob_2, this);
	Bind(EVT_UPDATE_VALUES, onUpdateValues, this);
}

void DoublePendulumFrame::onExit(wxCommandEvent &event)
{
	// delete dpObject;
	// delete drawingPanel;
	// delete monitorPanel;
	printf("onExit: I want to close thie app!!!!");
	Destroy();
}

void DoublePendulumFrame::onClose(wxCloseEvent &event)
{
	// delete dpObject;
	// delete drawingPanel;
	// delete monotorPanel;
	printf("onExit: I want to close thie app!!!!");
	Destroy();
}

void DoublePendulumFrame::onStart(wxCommandEvent &event)
{
	startEnabled = !startEnabled;
	if (startEnabled)
	{
		startBtn->SetLabelText(wxT("Stop"));
		pauseEnabled = false;
		pauseBtn->SetLabelText(wxT("Pause"));
		mainPanel->controlAction(mainPanel->START);
	}
	else
	{
		startBtn->SetLabelText(wxT("Start"));
		mainPanel->controlAction(mainPanel->STOP);
	}
}

void DoublePendulumFrame::onPause(wxCommandEvent &event)
{
	pauseEnabled = !pauseEnabled;
	if (pauseEnabled)
	{
		pauseBtn->SetLabelText(wxT("Run"));
		mainPanel->controlAction(mainPanel->PAUSE);
	}
	else
	{
		pauseBtn->SetLabelText(wxT("Pause"));
		mainPanel->controlAction(mainPanel->RUN);
	}
}

void DoublePendulumFrame::onToggleTrace(wxCommandEvent &event)
{
	traceEnabled = !traceEnabled;
	if (traceEnabled)
	{
		toggleTraceBtn->SetLabelText(wxT("Trace off"));
		mainPanel->controlAction(mainPanel->TRACE_ON);
	}
	else
	{
		toggleTraceBtn->SetLabelText(wxT("Trace on"));
		mainPanel->controlAction(mainPanel->TRACE_OFF);
	}
}

void DoublePendulumFrame::onClearTrace(wxCommandEvent &event)
{
	mainPanel->controlAction(mainPanel->TRACE_CLEAR);
}

void DoublePendulumFrame::onSwitchColor(wxCommandEvent &event)
{
	mainPanel->controlAction(mainPanel->SWITCHCOLOR);
}

void DoublePendulumFrame::onMassBob_1(wxCommandEvent &event)
{
	dpObject->setSettings(sMassBob_1->GetValue() / sliderFactor, -1, -1, -1, -1);
	mainPanel->controlAction(mainPanel->UPDATE_PANEL);
}

void DoublePendulumFrame::onMassBob_2(wxCommandEvent &event)
{
	dpObject->setSettings(-1, -1, sMassBob_2->GetValue() / sliderFactor, -1, -1);
	mainPanel->controlAction(mainPanel->UPDATE_PANEL);

}

void DoublePendulumFrame::onLengthBob_1(wxCommandEvent &event)
{
	dpObject->setSettings(-1, sLengthBob_1->GetValue() / sliderFactor, -1, -1, -1);
	mainPanel->controlAction(mainPanel->UPDATE_PANEL);
}

void DoublePendulumFrame::onLengthBob_2(wxCommandEvent &event)
{
	dpObject->setSettings(-1, -1, -1, sLengthBob_2->GetValue() / sliderFactor, - 1);
	mainPanel->controlAction(mainPanel->UPDATE_PANEL);
}

void DoublePendulumFrame::onUpdateValues(wxCommandEvent &event)
{
	float angle;
	angle = gonio_funcs::radToDegree(dpObject->status.theta1);
	theta1ValueBox->SetLabel(std::format("{:7.2f}", angle));
	angle = gonio_funcs::radToDegree(dpObject->status.theta2);
	theta2ValueBox->SetLabel(std::format("{:7.2f}", angle));
	timeLabel->SetLabel("time: " + std::format("{:.1f}", dpObject->status.time));
	if (startEnabled && !pauseEnabled)
	{
		monitorPanelTheta1->updateMonitor(dpObject->status);
	}
}
