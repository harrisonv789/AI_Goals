#pragma once
#include "GOAPAction.h"

class TestAction : public GOAPAction
{
protected:
	virtual void Reset() override;
	
public:

	// Constructor
	TestAction();

	// Destructor
	virtual ~TestAction() override;
	
	// Whether or not an action has finished executing
	virtual bool IsActionDone() override;

	// Pure virtual functions that the child classes must inherit
	virtual bool CheckProceduralPreconditions(AShip* _ship) override;

	// Performs the action
	virtual bool PerformAction(AShip* _ship, float _deltaTime) override;

	// Whether or not the action requires something in range
	virtual bool RequiresInRange() override;
};
