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
#include <stdlib.h>
#include <time.h>

SimulationWorld::SimulationWorld()
{
    for (int i = 0; i < 20; i++) {
        mGrid.resize(20);
        for (int x = 0; x < 20; x++) {
            mGrid[x].push_back(MachineState());
        }
    }
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

void SimulationWorld::RandomizeZombies()
{
    srand(static_cast<unsigned>(time(NULL)));
    
    for(int i = 0; i < 20;)
    {
        int x = rand() % 20;
        int y = rand() % 20;
        int z = rand() % 4;
        if(mGrid[x][y].mBeing == Being::EMPTY)
        {
            MachineState zombie = MachineState();
            zombie.mBeing = Being::ZOMBIE;
            switch (z) {
                case 0:
                    zombie.mFacing = MachineState::Facing::UP;
                    break;
                case 1:
                    zombie.mFacing = MachineState::Facing::LEFT;
                    break;
                case 2:
                    zombie.mFacing = MachineState::Facing::RIGHT;
                    break;
                default:
                case 3:
                    zombie.mFacing = MachineState::Facing::DOWN;
                    break;
            }
            zombie.mLocation.x = x;
            zombie.mLocation.y = y;
            mZombieMachine.BindState(zombie);
            mGrid[x][y] = zombie;
            mZombies.push_back(zombie);
            i++;
        }
    }
}

void SimulationWorld::RandomizeHumans()
{
    srand(static_cast<unsigned>(time(NULL)));
    
    for(int i = 0; i < 10;)
    {
        int x = rand() % 20;
        int y = rand() % 20;
        int z = rand() % 4;
        if(mGrid[x][y].mBeing == Being::EMPTY)
        {
            MachineState human = MachineState();
            human.mBeing = Being::HUMAN;
            switch (z) {
                case 0:
                    human.mFacing = MachineState::Facing::UP;
                    break;
                case 1:
                    human.mFacing = MachineState::Facing::LEFT;
                    break;
                case 2:
                    human.mFacing = MachineState::Facing::RIGHT;
                    break;
                default:
                case 3:
                    human.mFacing = MachineState::Facing::DOWN;
                    break;
            }
            human.mLocation.x = x;
            human.mLocation.y = y;
            mHumanMachine.BindState(human);
            mGrid[x][y] = human;
            mHumans.push_back(human);
            i++;
        }
    }
}

void SimulationWorld::SetZombieMachine(Machine<ZombieTraits> machine)
{
    mZombieMachine = machine;
}

void SimulationWorld::SetHumanMachine(Machine<HumanTraits> machine)
{
    mHumanMachine = machine;
}

void SimulationWorld::TakeTurn()
{
    for(auto &i : mZombies)
    {
        mZombieMachine.TakeTurn(i);
    }
    for(auto &i : mHumans)
    {
        mHumanMachine.TakeTurn(i);
    }
}

void SimulationWorld::RemoveHuman(Coord location)
{
    for(std::vector<MachineState>::iterator it = mHumans.begin(); it != mHumans.end(); it++)
    {
        if(it->mLocation.x == location.x && it->mLocation.y == location.y)
        {
            mHumans.erase(it);
            mGrid[location.x][location.y].mBeing = EMPTY;
            return;
        }
    }
    // Throw exception
}

void SimulationWorld::RemoveZombie(Coord location)
{
    for(std::vector<MachineState>::iterator it = mZombies.begin(); it != mZombies.end(); it++)
    {
        if(it->mLocation.x == location.x && it->mLocation.y == location.y)
        {
            mZombies.erase(it);
            mGrid[location.x][location.y].mBeing = EMPTY;
            return;
        }
    }
    // Throw exception
}

void SimulationWorld::ConvertHuman(Coord location)
{
    for(std::vector<MachineState>::iterator it = mHumans.begin(); it != mHumans.end(); it++)
    {
        if(it->mLocation.x == location.x && it->mLocation.y == location.y)
        {
            mGrid[location.x][location.y].mBeing = ZOMBIE;
            MachineState zombie = MachineState();
            zombie.mBeing = Being::ZOMBIE;
            zombie.mLocation = location;
            zombie.mFacing = it->mFacing;
            mZombieMachine.BindState(zombie);
            mZombies.push_back(zombie);
            mHumans.erase(it);
            return;
        }
    }
}

void SimulationWorld::Reset()
{
    for(std::vector<MachineState>::iterator it = mHumans.begin(); it != mHumans.end(); it++)
    {
        mGrid[it->mLocation.x][it->mLocation.y].mBeing = EMPTY;
    }
    mHumans.clear();
    for(std::vector<MachineState>::iterator it = mZombies.begin(); it != mZombies.end(); it++)
    {
        mGrid[it->mLocation.x][it->mLocation.y].mBeing = EMPTY;
    }
    mZombies.clear();
}
