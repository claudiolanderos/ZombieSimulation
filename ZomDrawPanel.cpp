//
//  ZomDrawPanel.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include "Machine.h"
#include "SimulationWorld.h"
#include <wx/listimpl.cpp>
#include <memory>
#include <sstream>

BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel)
	EVT_PAINT(ZomDrawPanel::PaintEvent)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void ZomDrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void ZomDrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void ZomDrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw the grid
	DrawGrid(dc);
}

void ZomDrawPanel::DrawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);
    
    dc.SetTextForeground(*wxRED);
    dc.DrawText("Zombies", 620, 10);
    std::stringstream ss;
    ss.str("");
    ss << "Program: ";
    ss << mZombiesProgram;
    dc.DrawText(ss.str(), 620, 20);
    ss.str("");
    ss << SimulationWorld::get().GetZombiesAlive();
    dc.DrawText(ss.str(), 620, 30);
    
    dc.SetTextForeground(*wxGREEN);
    dc.DrawText("Zombies", 620, 50);
    ss.str("");
    ss << "Program: ";
    ss << mHumansProgram;
    dc.DrawText(ss.str(), 620, 60);
    ss.str("");
    ss << SimulationWorld::get().GetHumansAlive();
    dc.DrawText(ss.str(), 620, 70);
    
    dc.SetTextForeground(*wxBLACK);
    ss.str("");
    ss << "Month: ";
    ss << SimulationWorld::get().GetMonth();
    dc.DrawText(ss.str(), 620, 100);
    // Draw grid outline
    dc.SetPen(*wxBLACK_PEN);
    int g = 30;
    for(int y = 0; y < 20; y++)
    {
        for(int x = 0; x < 20; x++)
        {
            dc.DrawRectangle(10+g*x,10+g*y, 30, 30);
            if(SimulationWorld::get().mGrid[x][y].mBeing == Being::ZOMBIE || SimulationWorld::get().mGrid[x][y].mBeing == Being::HUMAN)
            {
                wxPoint points[3];
                wxPoint point1;
                wxPoint point2;
                wxPoint point3;
                
                switch (SimulationWorld::get().mGrid[x][y].mFacing)
                {
                    case (MachineState::UP) :
                        point1 = wxPoint(30/2+g*x, g*y);
                        point2 = wxPoint(g*x,30+g*y);
                        point3 = wxPoint(30+g*x,30+g*y);
                        break;
                        
                    case (MachineState::RIGHT) :
                        point1 = wxPoint(30+g*x, 30/2+g*y);
                        point2 = wxPoint(g*x,g*y);
                        point3 = wxPoint(g*x, 30+g*y);
                        break;
                        
                    case (MachineState::DOWN) :
                        point1 = wxPoint(30/2+g*x, 30+g*y);
                        point2 = wxPoint(30+g*x,g*y);
                        point3 = wxPoint(g*x,g*y);
                        break;
                        
                    default:
                    case (MachineState::LEFT) :
                        point1 = wxPoint(g*x, 30/2+g*y);
                        point2 = wxPoint(30+g*x,g*y);
                        point3 = wxPoint(30+g*x,30+g*y);
                        break;
                }
                points[0] = point1;
                points[1] = point2;
                points[2] = point3;
                if(SimulationWorld::get().mGrid[x][y].mBeing == Being::ZOMBIE)
                {
                    dc.SetBrush(*wxRED_BRUSH);
                }
                else if(SimulationWorld::get().mGrid[x][y].mBeing == Being::HUMAN)
                {
                    dc.SetBrush(*wxGREEN_BRUSH);
                }
                dc.DrawPolygon(3, points, 10, 10);
                dc.SetBrush(*wxWHITE_BRUSH);
            }
        }
    }
}
