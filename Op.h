// Defines all the classes for different operations
// which can be performed.
#pragma once

struct MachineState;

// Abstract Base Class
class Op
{
public:
	Op(const char* OpName, int parameter)
		: mOpName(OpName)
		, mParam(parameter)
	{ }

	void DebugOutput(MachineState& state);

	virtual void Execute(MachineState& state) = 0;

	MachineState* GetTarget(MachineState& state, int n = 1);
	
	virtual ~Op() { }
protected:
	int mParam;
	const char* mOpName;
};

// Defines the rotate operation
// Param of 0 means rotate clockwise
// Param of 1 means rotate counter-clockwise
struct OpRotate : Op
{
	OpRotate(int parameter)
		: Op("OpRotate", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the goto operation
// Parameter determines the line number
struct OpGoto : Op
{
	OpGoto(int parameter)
		: Op("OpGoto", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

struct OpTestHuman : Op
{
    OpTestHuman(int parameter)
        : Op("OpTestHuman", parameter)
    { }
    
    virtual void Execute(MachineState& state) override;
};

struct OpTestZombie : Op
{
    OpTestZombie(int parameter)
    : Op("OpTestZombie", parameter)
    { }
    
    virtual void Execute(MachineState& state) override;
};

struct OpTestRandom : Op
{
    OpTestRandom()
    : Op("OpTestRandom", 0)
    { }
    
    virtual void Execute(MachineState& state) override;
};

struct OpTestPassable : Op
{
    OpTestPassable()
    : Op("OpTestPassable", 1)
    { }
    
    virtual void Execute(MachineState& state) override;
};

struct OpJe : Op
{
    OpJe(int parameter)
    : Op("OpJe", parameter)
    { }
    
    virtual void Execute(MachineState& state) override;
};

struct OpJne : Op
{
    OpJne(int parameter)
    : Op("OpJne", parameter)
    { }
    
    virtual void Execute(MachineState& state) override;
};

struct OpTestWall : Op
{
    OpTestWall()
        : Op("OpTestWall", 1)
    { }
    
    virtual void Execute(MachineState& state) override;
};

struct OpAttack : Op
{
    OpAttack()
    : Op("OpAttack", 1)
    { }
    
    virtual void Execute(MachineState& state) override;
};

struct OpRangedAttack : Op
{
    OpRangedAttack()
    : Op("OpRangedAttack", 2)
    { }
    
    virtual void Execute(MachineState& state) override;
};

struct OpForward : Op
{
    OpForward()
    : Op("OpForward", 1)
    { }
    
    virtual void Execute(MachineState& state) override;
};

struct OpEndTurn : Op
{
    OpEndTurn()
    : Op("OpEndTurn", 0)
    { }
    
    virtual void Execute(MachineState& state) override;
};
