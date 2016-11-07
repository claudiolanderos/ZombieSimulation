//
//  ZomFrame.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"
#include "SimulationWorld.h"

enum
{
	ID_SImSTART=1000,
	ID_TURN_TIMER,
    ID_LOAD_ZOMBIE,
    ID_LOAD_SURVIVOR,
    ID_RANDOMIZE
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame)
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SImSTART, ZomFrame::OnSimStart)
    EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::OnLoadZombie)
    EVT_MENU(ID_LOAD_SURVIVOR, ZomFrame::OnLoadSurvivor)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
, mIsActive(false)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(wxID_EXIT);
	
	// Simulation menu
	mSimMenu = new wxMenu;
	mSimMenu->Append(ID_SImSTART, "Start/stop\tSpace", "Start or stop the simulation");
    mSimMenu->Append(ID_LOAD_ZOMBIE, "Load Zombie", "Load zombies file");
    mSimMenu->Append(ID_LOAD_SURVIVOR, "Load Survivor", "Load survivors file");
    mSimMenu->Append(ID_RANDOMIZE, "Randomize Zombies", "Set random zombies in grid");
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(mSimMenu, "&Simulation");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new ZomDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	mTurnTimer = new wxTimer(this, ID_TURN_TIMER);

	// TEMP CODE: Initialize zombie test machine

	// END TEMP CODE
}

ZomFrame::~ZomFrame()
{
	delete mTurnTimer;
}

void ZomFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::OnNew(wxCommandEvent& event)
{
	// TODO: Add code for File>New
}

void ZomFrame::OnSimStart(wxCommandEvent& event)
{
	if (!mIsActive)
	{
		// Add timer to run once per second
		mTurnTimer->Start(1000);
		mIsActive = true;
	}
	else
	{
		mTurnTimer->Stop();
		mIsActive = false;
	}
}

void ZomFrame::OnTurnTimer(wxTimerEvent& event)
{
    SimulationWorld::get().TakeTurn();
    mPanel->PaintNow();
}

void ZomFrame::OnLoadZombie(wxCommandEvent &event)
{
    wxFileDialog
    openFileDialog(this, _(""), "./zom", "",
                   "ZOM Files (*.zom)|*.zom", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;     // the user changed idea...
    }
    
    openFileDialog.GetPath().ToStdString();
    
    Machine<ZombieTraits> zombieMachine;
    zombieMachine.LoadMachine(openFileDialog.GetPath().ToStdString());
    SimulationWorld::get().SetZombieMachine(zombieMachine);
}

void ZomFrame::OnLoadSurvivor(wxCommandEvent &event)
{
    wxFileDialog
    openFileDialog(this, _(""), "./zom", "",
                   "ZOM Files (*.zom)|*.zom", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;     // the user changed idea...
    }
    
    openFileDialog.GetPath().ToStdString();
    
    Machine<HumanTraits> humanMachine;
    humanMachine.LoadMachine(openFileDialog.GetPath().ToStdString());
    SimulationWorld::get().SetHumanMachine(humanMachine);
}

void ZomFrame::OnRandomize(wxCommandEvent &event)
{
    SimulationWorld::get().RandomizeZombies();
    SimulationWorld::get().RandomizeHumans();
}
