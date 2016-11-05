#include "Op.h"
#include "Machine.h"
#include "SimulationWorld.h"
#include "Traits.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	std::cout << state.mProgramCounter << ":" << mOpName << "," << mParam << std::endl;
}

void OpRotate::Execute(MachineState& state)
{
	DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::RIGHT;
		}
		else
		{
			state.mFacing = MachineState::LEFT;
		}
		break;
	case (MachineState::RIGHT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::DOWN;
		}
		else
		{
			state.mFacing = MachineState::UP;
		}
		break;
	case (MachineState::DOWN) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::LEFT;
		}
		else
		{
			state.mFacing = MachineState::RIGHT;
		}
		break;
	default:
	case (MachineState::LEFT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::UP;
		}
		else
		{
			state.mFacing = MachineState::DOWN;
		}
		break;
	}

	/*std::cout << "Now facing ";
	switch (state.mFacing)
	{
	case (MachineState::UP):
		std::cout << "UP";
		break;
	case (MachineState::RIGHT):
		std::cout << "RIGHT";
		break;
	case (MachineState::DOWN):
		std::cout << "DOWN";
		break;
	case (MachineState::LEFT):
		std::cout << "LEFT";
		break;
	}
	std::cout << std::endl;*/
    SimulationWorld::get().mGrid[state.mLocation.x][state.mLocation.y].mFacing = state.mFacing;
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
    // TODO excpetion
	DebugOutput(state);
    if(mParam > 0 && mParam < state.mProgramLength)
    {
        state.mProgramCounter = mParam;
    }
    else {
        // Throw exception
    }
}

void OpTestWall::Execute(MachineState &state)
{
    DebugOutput(state);
    state.mTest = (SimulationWorld::get().GetFacing(state.mLocation) == Being::WALL);
    state.mProgramCounter++;
}

void OpTestHuman::Execute(MachineState &state)
{
    DebugOutput(state);
    state.mTest = (SimulationWorld::get().GetFacing(state.mLocation, mParam) == Being::HUMAN);
    state.mProgramCounter++;
}

void OpTestZombie::Execute(MachineState &state)
{
    DebugOutput(state);
    state.mTest = (SimulationWorld::get().GetFacing(state.mLocation, mParam) == Being::ZOMBIE);
    state.mProgramCounter++;
}

void OpTestRandom::Execute(MachineState &state)
{
    // TODO
    DebugOutput(state);
    srand(static_cast<unsigned>(time(NULL)));
    state.mTest = rand() % 2;
    state.mProgramCounter++;
}

void OpTestPassable::Execute(MachineState &state)
{
    DebugOutput(state);
    state.mTest = (SimulationWorld::get().GetFacing(state.mLocation, mParam) == Being::EMPTY);
    state.mProgramCounter++;
}
void OpJe::Execute(MachineState &state)
{
    DebugOutput(state);
    if(mParam > 0 && mParam < state.mProgramLength)
    {
        if(state.mTest)
        {
            state.mProgramCounter = mParam;
        }
        else {
            state.mProgramCounter++;
        }
    }
    else {
        // Throw exception
    }
}

void OpJne::Execute(MachineState &state)
{
    DebugOutput(state);
    if(mParam > 0 && mParam < state.mProgramLength)
    {
        if(!state.mTest)
        {
            state.mProgramCounter = mParam;
        }
        else {
            state.mProgramCounter++;
        }
    }
    else {
        // Throw exception
    }
}

void OpAttack::Execute(MachineState &state)
{
    // TODO
}

void OpRangedAttack::Execute(MachineState &state)
{
    // TODO
}

void OpForward::Execute(MachineState &state)
{
    for(int y = 0; y < 20; y++)
    {
        for(int x = 0; x < 20; x++)
        {
            std::cout<< "|" << SimulationWorld::get().mGrid[x][y].mBeing;
        }
        std::cout<< std::endl << "-------------------------------------------" << std::endl;
    }
    DebugOutput(state);
    if(SimulationWorld::get().GetFacing(state.mLocation) == Being::EMPTY)
    {
        Coord currLocation = state.mLocation;
        switch (state.mFacing)
        {
            case (MachineState::UP) :
                state.mLocation.y--;
                break;
            case (MachineState::RIGHT) :
                state.mLocation.x++;
                break;
            case (MachineState::DOWN) :
                state.mLocation.y++;
                break;
            default:
            case (MachineState::LEFT) :
                state.mLocation.x--;
                break;
        }
        Coord newLocation = state.mLocation;
        SimulationWorld::get().MoveState(currLocation, newLocation);
    }
    state.mProgramCounter++;
    state.mActionsTaken++;
    
    for(int y = 0; y < 20; y++)
    {
        for(int x = 0; x < 20; x++)
        {
            std::cout<< "|" << SimulationWorld::get().mGrid[x][y].mBeing;
        }
        std::cout<< std::endl << "-------------------------------------------" << std::endl;
    }
}

void OpEndTurn::Execute(MachineState &state)
{
    int actions = state.GetActionsPerTurn();
    state.mActionsTaken = actions;
    state.mProgramCounter++;
}

