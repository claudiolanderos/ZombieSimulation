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

    SimulationWorld::get().mGrid[state.mLocation.x][state.mLocation.y].mFacing = state.mFacing;
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
	DebugOutput(state);
    if(mParam > 0 && mParam <= state.mProgramLength)
    {
        state.mProgramCounter = mParam;
    }
    else {
        throw InvalidOp();
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
    if(state.mProgramCounter > 25)
    {
        std::cout << "woop";
    }
    state.mTest = (SimulationWorld::get().GetFacing(state.mLocation, mParam) == Being::ZOMBIE);
    state.mProgramCounter++;
}

void OpTestRandom::Execute(MachineState &state)
{
    DebugOutput(state);
    srand(static_cast<unsigned>(time(NULL)));
    state.mTest = rand() % 2;
    state.mProgramCounter++;
}

void OpTestPassable::Execute(MachineState &state)
{
    DebugOutput(state);
    state.mTest = (SimulationWorld::get().GetFacing(state.mLocation) == Being::EMPTY);
    state.mProgramCounter++;
}
void OpJe::Execute(MachineState &state)
{
    DebugOutput(state);
    if(mParam > 0 && mParam <= state.mProgramLength)
    {
        if(state.mTest)
        {
            state.mProgramCounter = mParam;
        }
        else {
            if(mParam == state.mProgramLength)
            {
                throw InvalidOp();
            }
            state.mProgramCounter++;
        }
    }
    else {
        throw InvalidOp();
    }
}

void OpJne::Execute(MachineState &state)
{
    DebugOutput(state);
    if(mParam > 0 && mParam <= state.mProgramLength)
    {
        if(!state.mTest)
        {
            state.mProgramCounter = mParam;
        }
        else {
            if(mParam == state.mProgramLength)
            {
                throw InvalidOp();
            }
            state.mProgramCounter++;
        }
    }
    else {
        throw InvalidOp();
    }
}

void OpAttack::Execute(MachineState &state)
{
    Being being = SimulationWorld::get().GetFacing(state.mLocation);
    switch (being)
    {
        case Being::WALL:
        case Being::EMPTY:
            break;
        case Being::ZOMBIE:
        case Being::HUMAN:
            Coord victimLocation = state.mLocation;
            switch (state.mFacing)
            {
                case (MachineState::UP) :
                    victimLocation.y -= 1;
                    break;
                case (MachineState::RIGHT):
                    victimLocation.x += 1;
                    break;
                case (MachineState::DOWN):
                    victimLocation.y += 1;
                    break;
                default:
                case (MachineState::LEFT):
                    victimLocation.x -= 1;
                    break;
            }
            if(state.mBeing == Being::HUMAN)
            {
                if(being == Being::HUMAN)
                {
                    SimulationWorld::get().RemoveHuman(victimLocation);
                }
                else {
                    SimulationWorld::get().RemoveZombie(victimLocation);
                }
            }
            else {
                if(being == Being::HUMAN)
                {
                    SimulationWorld::get().ConvertHuman(victimLocation);
                }
            }
            break;
    }
    state.mProgramCounter++;
    state.mActionsTaken++;
}

void OpRangedAttack::Execute(MachineState &state)
{
    DebugOutput(state);
    if(state.mBeing != Being::HUMAN)
    {
        throw InvalidOp();
    }
    Being being = SimulationWorld::get().GetFacing(state.mLocation, mParam);
    if(being == Being::HUMAN || being == Being::ZOMBIE)
    {
        Coord victimLocation = state.mLocation;
        switch (state.mFacing)
        {
            case (MachineState::UP) :
                victimLocation.y -= 2;
                break;
            case (MachineState::RIGHT) :
                victimLocation.x += 2;
                break;
            case (MachineState::DOWN) :
                victimLocation.y += 2;
                break;
            default:
            case (MachineState::LEFT) :
                victimLocation.x -= 2;
                break;
        }
        if(being == Being::HUMAN)
        {
            SimulationWorld::get().RemoveHuman(victimLocation);
        }
        else{
            SimulationWorld::get().RemoveZombie(victimLocation);
        }
    }
    state.mProgramCounter++;
    state.mActionsTaken++;
}

void OpForward::Execute(MachineState &state)
{
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
}

void OpEndTurn::Execute(MachineState &state)
{
    int actions = state.GetActionsPerTurn();
    state.mActionsTaken = actions;
    state.mProgramCounter++;
}

