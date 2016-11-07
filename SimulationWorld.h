//
//  SimulationWorld.hpp
//  zombiewx-mac
//
//  Created by Claudio Landeros on 11/3/16.
//  Copyright © 2016 Sanjay Madhav. All rights reserved.
//

#include "Singleton.h"
#include "Traits.h"
#include "Machine.h"
#include <vector>
#pragma once

class SimulationWorld : public Singleton<SimulationWorld>
{
    DECLARE_SINGLETON(SimulationWorld);

public:
    
    Being GetFacing(Coord coord, int n = 1);
    bool IsWall(Coord coord);
    bool IsOccupied(Coord coord);
    
    void SetBeing(Being being, Coord coord);
    Being GetBeing(Coord coord);
    void MoveState(Coord currLocation, Coord newLocation);
    
    void SetHumanMachine(Machine<HumanTraits> machine);
    void SetZombieMachine(Machine<ZombieTraits> machine);
    
    void RandomizeZombies();
    void RandomizeHumans();
    
    void ConvertHuman(Coord location);
    
    void RemoveZombie(Coord location);
    void RemoveHuman(Coord location);
    
    void TakeTurn();
    
    void Reset();
    
    long GetZombiesAlive() { return mZombies.size(); }
    long GetHumansAlive() { return mHumans.size(); }
    
    // World grid
    std::vector<std::vector<MachineState>> mGrid;
    
protected:
    SimulationWorld();
    
    std::vector<MachineState> mZombies;
    std::vector<MachineState> mHumans;

    Machine<ZombieTraits> mZombieMachine;
    Machine<HumanTraits> mHumanMachine;;
};
