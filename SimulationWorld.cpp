//
//  SimulationWorld.cpp
//  zombiewx-mac
//
//  Created by Claudio Landeros on 11/3/16.
//  Copyright © 2016 Sanjay Madhav. All rights reserved.
//

#include "SimulationWorld.h"
#include "Traits.h"
#include <stdexcept>

SimulationWorld::SimulationWorld()
{
    for (int i = 0; i < 20; i++) {
        mGrid.resize(20);
        for (int x = 0; x < 20; x++) {
            mGrid[x].push_back(MachineState());
        }
    }
    // TEMP CODE
    MachineState state = MachineState();
    state.mBeing = ZOMBIE;
    mGrid[5][5] = state;
    
}
Being SimulationWorld::GetFacing(Coord coord, int n)
{
    Being being;
    MachineState character = mGrid[coord.x][coord.y];
    if(character.mBeing == Being::EMPTY || character.mBeing == Being::WALL)
    {
        // TODO throw exception
    }
    else {
        MachineState::Facing facing = character.mFacing;
        
        try {
        switch (facing)
        {
            case (MachineState::UP) :
                if(coord.y-n < 0)
                {
                    being = Being::WALL;
                    break;
                }
                being = mGrid[coord.x][coord.y-n].mBeing;
                break;
            case (MachineState::RIGHT) :
                if(coord.x+n >= mGrid.size())
                {
                    being = Being::WALL;
                    break;
                }
                being =  mGrid[coord.x+n][coord.y].mBeing;
                break;
            case (MachineState::DOWN) :
                if(coord.y+n >= mGrid.size())
                {
                    being = Being::WALL;
                    break;
                }
                being =  mGrid[coord.x][coord.y+n].mBeing;
                break;
            default:
            case (MachineState::LEFT) :
                if(coord.x-n < 0)
                {
                    being = Being::WALL;
                    break;
                }
                being =  mGrid[coord.x-n][coord.y].mBeing;
                break;
        }
        } catch(const std::out_of_range& oor) {
            being = Being::WALL;
        }
    }
    return being;
}

void SimulationWorld::SetBeing(Being being, Coord coord)
{
    mGrid[coord.x][coord.y].mBeing =  being;
}

void SimulationWorld::MoveState(Coord currLocation, Coord newLocation)
{
    MachineState state = mGrid[currLocation.x][currLocation.y];
    mGrid[currLocation.x][currLocation.y] = MachineState();
    mGrid[newLocation.x][newLocation.y] = state;
}
