#ifndef DP_FRAME_H
#define DP_FRAME_H
#include <wx/wx.h>

class DrawingPane;

class DoublePendulumFrame : public wxFrame
{
private:
	DrawingPane *drawingPane;

public:
	DoublePendulumFrame();
	~DoublePendulumFrame();
	void onClose(wxCloseEvent &event);
	void onExit(wxCommandEvent &event);
	void onStart(wxCommandEvent &event);
	void onStop(wxCommandEvent &event);
	void onSwitchColor(wxCommandEvent &event);
	void onClearTrace(wxCommandEvent &event);
	void onToggleTrace(wxCommandEvent &event);

	DECLARE_EVENT_TABLE()
};

enum
{
	BUTTON_START = wxID_HIGHEST + 1,
	BUTTON_STOP = wxID_HIGHEST + 2,
	BUTTON_CLEARTRACE = wxID_HIGHEST + 3,
	BUTTON_TOGGLETRACE = wxID_HIGHEST + 4,
	BUTTON_SWITCHCOLOR = wxID_HIGHEST + 5
};

#endif // DP_FRAME_H
