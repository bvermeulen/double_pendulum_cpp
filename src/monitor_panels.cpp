#include <config.h>
#include <monitor_panels.h>

MonitorPanelThetas::MonitorPanelThetas(wxFrame *parent, int monitorFrameRate) : wxPanel(parent)
{
    timeTheta1Plot = new mpFXYVector();
    timeTheta1Plot->SetContinuity(true);
    wxPen xyPen1(*wxRED, 2, wxPENSTYLE_SOLID);
    timeTheta1Plot->SetPen(xyPen1);
    timeTheta1Plot->SetDrawOutsideMargins(false);

    timeTheta2Plot = new mpFXYVector();
    timeTheta2Plot->SetContinuity(true);
    wxPen xyPen2(*wxBLUE, 2, wxPENSTYLE_SOLID);
    timeTheta2Plot->SetPen(xyPen2);
    timeTheta2Plot->SetDrawOutsideMargins(false);


    mpPlotLeft = new mpWindow(this, -1, wxPoint(0, 0), wxSize(MONITOR_MIN_SIZE_WIDTH, MONITOR_MIN_SIZE_HEIGHT));
    mpPlotLeft->SetMargins(20, 10, 40, 10);

    wxFont graphFont(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    mpScaleX *xaxis = new mpScaleX(wxT(""), mpALIGN_BOTTOM, true);
    mpScaleY *yaxis = new mpScaleY(wxT(""), mpALIGN_LEFT, true);
    xaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    xaxis->SetAuto(false);
    xaxis->SetMinScale(-MONITOR_DURATION);
    xaxis->SetMaxScale(0.0);
    yaxis->SetFont(graphFont);
    yaxis->SetDrawOutsideMargins(false);
    yaxis->SetAuto(false);
    yaxis->SetMinScale(MONITOR_MIN_Y);
    yaxis->SetMaxScale(MONITOR_MAX_Y);

    mpPlotLeft->AddLayer(xaxis);
    mpPlotLeft->AddLayer(yaxis);
    mpPlotLeft->AddLayer(timeTheta1Plot);
    mpPlotLeft->AddLayer(timeTheta2Plot);
    mpPlotLeft->EnableDoubleBuffer(true);
    mpPlotLeft->SetMPScrollbars(false);
    mpPlotLeft->Fit();

    int samples = int(1000 / monitorFrameRate);
    for (int i=0; i < samples; i++)
        vectorTime.push_back(-MONITOR_DURATION * float(i) / float(samples));
    vectorTheta1.resize(samples, 0.0);
    vectorTheta2.resize(samples, 0.0);
}

MonitorPanelThetas::~MonitorPanelThetas()
{
    delete timeTheta1Plot;
    delete timeTheta2Plot;
    delete mpPlotLeft;
}

void MonitorPanelThetas::updateMonitor(float theta1, wxColor& color1, float theta2, wxColor& color2)
{
    vectorTheta1.push_back(theta1);
    std::rotate(vectorTheta1.rbegin(), vectorTheta1.rbegin() + 1, vectorTheta1.rend());
    vectorTheta1.pop_back();
    
    wxPen xyPen1(color1, 2, wxPENSTYLE_SOLID);
    timeTheta1Plot->SetPen(xyPen1);
    timeTheta1Plot->SetData(vectorTime, vectorTheta1);

    vectorTheta2.push_back(theta2);
    std::rotate(vectorTheta2.rbegin(), vectorTheta2.rbegin() + 1, vectorTheta2.rend());
    vectorTheta2.pop_back();
    
    wxPen xyPen2(color2, 2, wxPENSTYLE_SOLID);
    timeTheta2Plot->SetPen(xyPen2);
    timeTheta2Plot->SetData(vectorTime, vectorTheta2);
    mpPlotLeft->UpdateAll();
}



