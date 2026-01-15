#ifndef DP_FRAME_H
#define DP_FRAME_H
#include <wx/wx.h>
#include <doublependulum.h>

class MainPanel;
class MonitorPanelTheta1;

class DoublePendulumFrame : public wxFrame
{
private:
	DoublePendulum *dpObject;
	MainPanel *mainPanel;
	MonitorPanelTheta1 *monitorPanelTheta1;
	float sliderFactor;
	wxSize sliderSize;
	wxButton *startBtn;
	wxButton *pauseBtn;
	wxButton *toggleTraceBtn;
	wxSlider *sMassBob_1;
	wxSlider *sMassBob_2;
	wxSlider *sLengthBob_1;
	wxSlider *sLengthBob_2;
	wxStaticText *timeLabel;
	bool startEnabled, pauseEnabled, traceEnabled;
	void initUI();
	void onClose(wxCloseEvent &event);
	void onExit(wxCommandEvent &event);
	void onStart(wxCommandEvent &event);
	void onPause(wxCommandEvent &event);
	void onToggleTrace(wxCommandEvent &event);
	void onClearTrace(wxCommandEvent &event);
	void onSwitchColor(wxCommandEvent &event);
	void onMassBob_1(wxCommandEvent &event);
	void onMassBob_2(wxCommandEvent &event);
	void onLengthBob_1(wxCommandEvent &event);
	void onLengthBob_2(wxCommandEvent &event);
	void onUpdateValues(wxCommandEvent &event);

public:
	DoublePendulumFrame();
	~DoublePendulumFrame();

};

#endif // DP_FRAME_H
