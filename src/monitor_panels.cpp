#include <monitor_panels.h>

MonitorPanelTheta1::MonitorPanelTheta1(wxFrame *parent, int monitorFrameRate) : wxPanel(parent)
{
    timeTheta1Plot = new mpFXYVector();
    timeTheta1Plot->SetContinuity(true);
    wxPen xyPen(*wxRED, 2, wxPENSTYLE_SOLID);
    timeTheta1Plot->SetPen(xyPen);
    timeTheta1Plot->SetDrawOutsideMargins(false);

    mpPlotTheta1 = new mpWindow(this, -1, wxPoint(0, 0), wxSize(400, 200));
    mpPlotTheta1->SetMargins(20, 10, 40, 10);

    wxFont graphFont(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    mpScaleX *xaxis = new mpScaleX(wxT(""), mpALIGN_BOTTOM, true);
    mpScaleY *yaxis = new mpScaleY(wxT(""), mpALIGN_LEFT, true);
    xaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    xaxis->SetAuto(false);
    xaxis->SetMinScale(-40.0);
    xaxis->SetMaxScale(0.0);
    yaxis->SetFont(graphFont);
    yaxis->SetDrawOutsideMargins(false);
    yaxis->SetAuto(false);
    yaxis->SetMinScale(-2100.0);
    yaxis->SetMaxScale(+2100.0);

    mpPlotTheta1->AddLayer(xaxis);
    mpPlotTheta1->AddLayer(yaxis);
    mpPlotTheta1->AddLayer(timeTheta1Plot);
    mpPlotTheta1->EnableDoubleBuffer(true);
    mpPlotTheta1->SetMPScrollbars(false);
    mpPlotTheta1->Fit();

    int samples = int(1000 / monitorFrameRate);
    for (int i=0; i < samples; i++)
        vectorTime.push_back(-40.0 * float(i) / float(samples));
    vectorTheta1.resize(samples, 0.0);
}

MonitorPanelTheta1::~MonitorPanelTheta1()
{
    delete timeTheta1Plot;
}

void MonitorPanelTheta1::updateMonitor(float time)
{
    float ycoord = 1500.0 * sin(time * 3.0);
    vectorTheta1.push_back(ycoord);
    std::rotate(vectorTheta1.rbegin(), vectorTheta1.rbegin() + 1, vectorTheta1.rend());
    vectorTheta1.pop_back();
    
    timeTheta1Plot->SetData(vectorTime, vectorTheta1);
    mpPlotTheta1->UpdateAll();
}



