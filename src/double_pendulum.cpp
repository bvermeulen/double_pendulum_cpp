#include <wx/wx.h>
#include <wx/sizer.h>
#include <drawing_pane.h>

class MyApp : public wxApp
{
	wxFrame *frame;
	BasicDrawPane *drawPane;

	bool OnInit()
	{
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		frame = new wxFrame((wxFrame *)NULL, -1, wxT("Double Pendulum"), wxPoint(50, 10), wxSize(1000, 800));

		drawPane = new BasicDrawPane((wxFrame *)frame);
		sizer->Add(drawPane, 1, wxEXPAND);

		frame->SetSizer(sizer);
		frame->SetAutoLayout(true);

		frame->Show();
		return true;
	};
};

IMPLEMENT_APP(MyApp)
