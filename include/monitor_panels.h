#ifndef MONITORPANELS_H
#define MONITORPANELS_H
#include <wx/wx.h>
#include <mathplot.h>


class MonitorPanelTheta1 : public wxPanel
{
	private:
        int monitorFrameRate;
        mpFXYVector *timeTheta1Plot;
        std::vector<double> vectorTime;
        std::vector<double> vectorTheta1;
        mpWindow *mpPlotTheta1;
    
    public:
        MonitorPanelTheta1(wxFrame *parent, int monitorFrameRate);
        ~MonitorPanelTheta1();
        void updateMonitor(float time);
};

#endif // MONITORPANELS_H