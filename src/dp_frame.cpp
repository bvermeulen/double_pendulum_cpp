#include <wx/sizer.h>
#include <wx/wx.h>
#include <dp_frame.h>
#include <drawing_pane.h>

BEGIN_EVENT_TABLE(DoublePendulumFrame, wxFrame)
EVT_CLOSE(DoublePendulumFrame::onClose)
EVT_BUTTON(BUTTON_START, DoublePendulumFrame::onStart)
EVT_BUTTON(BUTTON_STOP, DoublePendulumFrame::onStop)
EVT_BUTTON(BUTTON_CLEARTRACE, DoublePendulumFrame::onClearTrace)
EVT_BUTTON(BUTTON_TOGGLETRACE, DoublePendulumFrame::onToggleTrace)
EVT_BUTTON(BUTTON_SWITCHCOLOR, DoublePendulumFrame::onSwitchColor)

END_EVENT_TABLE()

DoublePendulumFrame::DoublePendulumFrame() : wxFrame((wxFrame *)NULL, -1, wxT("Double Pendulum"), wxPoint(0, 0), wxSize(800, 620))
{
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
	drawingPane = new DrawingPane(this);
	drawingPane->SetMinSize(wxSize(600, 400));
	// drawingPane->SetMaxSize(wxSize(1000, 800));
	wxPanel *leftPanel = new wxPanel(this);
	leftPanel->SetMinSize(wxSize(100, 400));
	leftPanel->SetMaxSize(wxSize(200, 800));
	mainBox->Add(leftPanel, true, wxEXPAND | wxTOP | wxLEFT, 10);
	mainBox->AddSpacer(10);
	mainBox->Add(drawingPane, true, wxEXPAND | wxTOP | wxRIGHT, 10);

	// add items in graphBox
	wxPanel *theta1Pane = new wxPanel(this);
	theta1Pane->SetMinSize(wxSize(400, 100));
	wxPanel *theta2Pane = new wxPanel(this);
	theta2Pane->SetMinSize(wxSize(400, 100));
	graphBox->Add(theta1Pane, true, wxEXPAND | wxTOP | wxLEFT, 10);
	graphBox->AddSpacer(10);
	graphBox->Add(theta2Pane, true, wxEXPAND | wxTOP | wxRIGHT, 10);

	// add items in controlBox
	wxButton *startBtn = new wxButton(this, BUTTON_START, wxT("Start"));
	wxButton *stopBtn = new wxButton(this, BUTTON_STOP, wxT("Stop"));
	wxButton *clearTraceBtn = new wxButton(this, BUTTON_CLEARTRACE, wxT("Clear trace"));
	wxButton *toggleTraceBtn = new wxButton(this, BUTTON_TOGGLETRACE, wxT("Trace on/ off"));
	wxButton *switchColorBtn = new wxButton(this, BUTTON_SWITCHCOLOR, wxT("Switch color"));
	controlBox->Add(startBtn, true, wxTOP | wxBOTTOM | wxLEFT, 10);
	controlBox->Add(stopBtn, true, wxTOP | wxBOTTOM, 10);
	controlBox->Add(clearTraceBtn, true, wxTOP | wxBOTTOM, 10);
	controlBox->Add(toggleTraceBtn, true, wxTOP | wxBOTTOM, 10);
	controlBox->Add(switchColorBtn, true, wxTOP | wxBOTTOM, 10);

	// collate in the vertical box
	vbox->Add(mainBox, wxEXPAND);
	vbox->Add(graphBox);
	vbox->Add(controlBox);

	this->SetSizer(vbox);
	this->Show();
	this->Centre();
}

DoublePendulumFrame::~DoublePendulumFrame()
{
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
	drawingPane->controlAction(drawingPane->START);
}

void DoublePendulumFrame::onStop(wxCommandEvent &event)
{
	drawingPane->controlAction(drawingPane->STOP);
}

void DoublePendulumFrame::onSwitchColor(wxCommandEvent &event)
{
	drawingPane->controlAction(drawingPane->SWITCHCOLOR);
}

void DoublePendulumFrame::onClearTrace(wxCommandEvent &event)
{
	drawingPane->controlAction(drawingPane->CLEARTRACE);
}

void DoublePendulumFrame::onToggleTrace(wxCommandEvent &event)
{
	drawingPane->controlAction(drawingPane->TOGGLETRACE);
}

//void DoublePendulumFrame::onSwitchColor(wxCommandEvent &event)
//{
//	drawingPane->controlAction(drawingPane->SWITCHCOLOR);
//}