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
	ID_SIMSTART=1000,
	ID_TURN_TIMER,
    ID_LOAD_ZOMBIE,
    ID_LOAD_SURVIVOR,
    ID_RANDOMIZE,
    ID_RESET
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame)
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SIMSTART, ZomFrame::OnSimStart)
    EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::OnLoadZombie)
    EVT_MENU(ID_LOAD_SURVIVOR, ZomFrame::OnLoadSurvivor)
    EVT_MENU(ID_RESET, ZomFrame::OnReset)
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
	mSimMenu->Append(ID_SIMSTART, "Start/stop\tSpace", "Start or stop the simulation");
    mSimMenu->Append(ID_LOAD_ZOMBIE, "Load Zombie", "Load zombies file");
    mSimMenu->Append(ID_LOAD_SURVIVOR, "Load Survivor", "Load survivors file");
    mSimMenu->Append(ID_RANDOMIZE, "Randomize Zombies", "Set random zombies in grid");
    mSimMenu->Append(ID_RESET, "Reset Simulation");
    
    mSimMenu->Enable(ID_SIMSTART, false);
    mSimMenu->Enable(ID_RANDOMIZE, false);
    mSimMenu->Enable(ID_RESET, false);

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
    mTurnTimer->Stop();
    mPanel->mHumansProgram = "";
    mPanel->mZombiesProgram = "";
    SimulationWorld::get().Reset();
    mPanel->PaintNow();
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
    mPanel->PaintNow();
    if(SimulationWorld::get().GetHumansAlive() == 0)
    {
        wxMessageBox("Zombies Win!", "Game Over", wxOK | wxICON_EXCLAMATION);
        mTurnTimer->Stop();
        return;
    }
    else if(SimulationWorld::get().GetZombiesAlive() == 0)
    {
        wxMessageBox("Humans Win!", "Game Over", wxOK | wxICON_EXCLAMATION);
        mTurnTimer->Stop();
        return;
    }
    SimulationWorld::get().TakeTurn();
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
    mPanel->mZombiesProgram = openFileDialog.GetFilename().ToStdString();
    Machine<ZombieTraits> zombieMachine;
    zombieMachine.LoadMachine(openFileDialog.GetPath().ToStdString());
    SimulationWorld::get().SetZombieMachine(zombieMachine);
    
    mZombieLoaded = true;
    if(mHumanLoaded)
    {
        mSimMenu->Enable(ID_SIMSTART, true);
        mSimMenu->Enable(ID_RESET, true);
        mSimMenu->Enable(ID_RANDOMIZE, true);
    }
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
    mPanel->mHumansProgram =     openFileDialog.GetFilename().ToStdString();
    Machine<HumanTraits> humanMachine;
    humanMachine.LoadMachine(openFileDialog.GetPath().ToStdString());
    SimulationWorld::get().SetHumanMachine(humanMachine);
    
    mHumanLoaded = true;
    if(mZombieLoaded)
    {
        mSimMenu->Enable(ID_SIMSTART, true);
        mSimMenu->Enable(ID_RESET, true);
        mSimMenu->Enable(ID_RANDOMIZE, true);
    }
}

void ZomFrame::OnRandomize(wxCommandEvent &event)
{
    SimulationWorld::get().RandomizeZombies();
    SimulationWorld::get().RandomizeHumans();
}

void ZomFrame::OnReset(wxCommandEvent &event)
{
    mTurnTimer->Stop();
    SimulationWorld::get().Reset();
    mPanel->PaintNow();
}
