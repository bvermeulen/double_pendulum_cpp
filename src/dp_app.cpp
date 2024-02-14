#include <wx/wx.h>
#include <wx/sizer.h>
#include <dp_frame.h>

class MyApp : public wxApp
{
	DoublePendulumFrame *frame;

	bool OnInit()
	{
		frame = new DoublePendulumFrame();
		frame->Show();
		return true;
	}
};

IMPLEMENT_APP(MyApp)
