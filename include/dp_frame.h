#ifndef DP_FRAME_H
#define DP_FRAME_H
#include <wx/wx.h>

class DrawingPanel;

class DoublePendulumFrame : public wxFrame
{
private:
	float sliderFactor;
	wxSize sliderSize;
	DrawingPanel *drawingPanel;
	wxButton *startBtn;
	wxButton *pauseBtn;
	wxButton *toggleTraceBtn;
	wxSlider *sMassBob_1;
	wxSlider *sMassBob_2;
	wxSlider *sLengthBob_1;
	wxSlider *sLengthBob_2;
	bool startEnabled, pauseEnabled, traceEnabled;

public:
	DoublePendulumFrame();
	~DoublePendulumFrame();
	void initUI();
	void onClose(wxCloseEvent &event);
	void onExit(wxCommandEvent &event);
	void onStart(wxCommandEvent &event);
	void onPause(wxCommandEvent &event);
	void onToggleTrace(wxCommandEvent &event);
	void onClearTrace(wxCommandEvent &event);
	void onSwitchColor(wxCommandEvent &event);
	void onMassBob_1(wxCommandEvent &evennt);
	void onMassBob_2(wxCommandEvent &evennt);
	void onLengthBob_1(wxCommandEvent &evennt);
	void onLengthBob_2(wxCommandEvent &evennt);

	DECLARE_EVENT_TABLE()
};

enum
{
	BUTTON_START = wxID_HIGHEST + 1,
	BUTTON_PAUSE = wxID_HIGHEST + 2,
	BUTTON_TOGGLETRACE = wxID_HIGHEST + 3,
	BUTTON_CLEARTRACE = wxID_HIGHEST + 4,
	BUTTON_SWITCHCOLOR = wxID_HIGHEST + 5,
	MASSBOB_1 = wxID_HIGHEST + 6,
	MASSBOB_2 = wxID_HIGHEST + 7,
	LENGTHBOB_1 = wxID_HIGHEST + 8,
	LENGTHBOB_2 = wxID_HIGHEST + 9,
	DAMPING = wxID_HIGHEST + 10,
};

#endif // DP_FRAME_H
