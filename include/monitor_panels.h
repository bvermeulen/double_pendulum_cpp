#ifndef MONITORPANELS_H
#define MONITORPANELS_H
#include <wx/wx.h>
#include <mathplot.h>
#include <doublependulum.h>

class MonitorPanelThetas : public wxPanel
{
	private:
        int monitorFrameRate;
        mpFXYVector *timeTheta1Plot;
        mpFXYVector *timeTheta2Plot;
        std::vector<double> vectorTime;
        std::vector<double> vectorTheta1;
        std::vector<double> vectorTheta2;
        mpWindow *mpPlotLeft;
    
    public:
        MonitorPanelThetas(wxFrame *parent, int monitorFrameRate);
        ~MonitorPanelThetas();
        void updateMonitor(float theta1, const wxColor* color1, float theta2, const wxColor* color2);
};

#endif // MONITORPANELS_H