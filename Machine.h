#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <fstream>
#include "Op.h"
#include "Exceptions.h"
#include "Traits.h"

// Defines state data accessible by the machine and ops
struct MachineState
{
	// Friend Machine so BindState works
	template <typename MachineTraits>
	friend class Machine;

	enum Facing { UP, RIGHT, DOWN, LEFT };

	MachineState()
		: mProgramCounter(1)
        , mProgramLength(1)
		, mActionsTaken(0)
		, mFacing(UP)
        , mBeing(EMPTY)
		, mTest(false)
	{ }
    
	~MachineState()
	{
	}

    // Lenght of behaviour program
    int mProgramLength;
	// Active line number in behavior program
	int mProgramCounter;
	// Number of actions taken this turn
	int mActionsTaken;
	// Current facing of this character
	Facing mFacing;
    // Current location of this character
    Coord mLocation;
    // Type of character
    Being mBeing;
	// Test flag for branches
	bool mTest;

	int GetActionsPerTurn() const noexcept { return mActionsPerTurn; }
	bool GetInfect() const noexcept { return mInfectOnAttack; }
private:
    
	// Data which is set by the traits
	int mActionsPerTurn;
	bool mInfectOnAttack;
};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
public:
	// Load in all the ops for this machine from the specified file
	void LoadMachine(const std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);

	// Destructor
	~Machine();
    
private:
	std::vector<std::shared_ptr<Op>> mOps;
};

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const std::string& filename)
{
    mOps.clear();
    
    std::ifstream::pos_type size;
    // Open the file for input and start ATE (at the end)
    std::ifstream file (filename, std::ios::in|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg(); // Save the size of the file
        file.seekg (0, std::ios::beg); // Seek back to start of file
        
        // Validate header
        std::string line;
        std::string operation;
        
        while (std::getline(file, line)){
            operation.clear();
            for(int i = 0; i < line.length(); i++)
            {
                operation += line[i];
                
                if(operation == "test_wall")
                {
                    mOps.push_back(std::make_shared<OpTestWall>());
                    break;
                }
                else if(operation == "test_random")
                {
                    mOps.push_back(std::make_shared<OpTestRandom>());
                    break;
                }
                else if(operation == "test_passable")
                {
                    mOps.push_back(std::make_shared<OpTestPassable>());
                    break;
                }
                else if(operation == "attack")
                {
                    mOps.push_back(std::make_shared<OpAttack>());
                    break;
                }
                else if(operation == "ranged_attack")
                {
                    mOps.push_back(std::make_shared<OpRangedAttack>());
                    break;
                }
                else if(operation == "forward")
                {
                    mOps.push_back(std::make_shared<OpForward>());
                    break;
                }
                else if(operation == "endturn")
                {
                    mOps.push_back(std::make_shared<OpEndTurn>());
                    break;
                }
                else if(operation == "rotate")
                {
                    if(line[++i] != ',')
                    {
                        //TODO throw parsing exception
                    }
                    else {
                        i++;
                        if(line[i] != '0' && line[i] != '1')
                        {
                            // TODO throw parsing exception
                        }
                        else {
                            mOps.push_back(std::make_shared<OpRotate>(line[i] - '0'));
                            break;
                        }
                    }
                }
                else if(operation == "test_human")
                {
                    if(line[++i] != ',')
                    {
                        //TODO throw parsing exception
                    }
                    else {
                        i++;
                        if(line[i] != '1' && line[i] != '2')
                        {
                            // TODO throw parsing exception
                        }
                        else {
                            mOps.push_back(std::make_shared<OpTestHuman>(line[i] - '0'));
                            break;
                        }
                    }
                }
                else if(operation == "test_zombie")
                {
                    if(line[++i] != ',')
                    {
                        //TODO throw parsing exception
                    }
                    else {
                        i++;
                        if(line[i] == '1' || line[i] == '2')
                        {
                            mOps.push_back(std::make_shared<OpTestZombie>(line[i] - '0'));
                            break;                        }
                        else {
                            // Throw exception
                        }
                    }
                }
                else if(operation == "goto")
                {
                    if(line[++i] != ',')
                    {
                        //TODO throw parsing exception
                    }
                    else {
                        std::string number;
                        i++;
                        while(line[i] != '\n' && line[i] != ';')
                        {
                            number += line[i];
                            i++;
                        }
                        try {
                            int x = std::stoi(number, nullptr, 10);
                            mOps.push_back(std::make_shared<OpGoto>(x));
                            break;
                        } catch(std::exception e){
                            // TODO throw parsing exception
                        }
                    }
                }
                else if(operation == "je")
                {
                    if(line[++i] != ',')
                    {
                        //TODO throw parsing exception
                    }
                    else {
                        std::string number;
                        i++;
                        while(line[i] != '\0' && line[i] != '\n' && line[i] != ';')
                        {
                            number += line[i];
                            i++;
                        }
                        try {
                            int x = std::stoi(number, nullptr, 10);
                            mOps.push_back(std::make_shared<OpJe>(x));
                            break;
                        } catch(std::exception e){
                            // TODO throw parsing exception
                        }
                    }
                }
                else if(operation == "jne")
                {
                    if(line[++i] != ',')
                    {
                        //TODO throw parsing exception
                    }
                    else {
                        std::string number;
                        i++;
                        while(line[i] != '\n' && line[i] != ';')
                        {
                            number += line[i];
                            i++;
                        }
                        try {
                            int x = std::stoi(number, nullptr, 10);
                            mOps.push_back(std::make_shared<OpJne>(x));
                            break;
                        } catch(std::exception e){
                            // TODO throw parsing exception
                        }
                    }
                }
            }
        }
        file.close();
        
        file.get();
    }
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.mActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.mInfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
    state.mProgramLength = static_cast<int>(mOps.size());
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	std::cout << "TAKING TURN" << std::endl;
	state.mActionsTaken = 0;
	while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		mOps.at(state.mProgramCounter - 1)->Execute(state);
	}
}

template <typename MachineTraits>
Machine<MachineTraits>::~Machine()
{
	mOps.clear();
}
