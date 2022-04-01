#pragma once

// Forward declaration of the ship class
class AShip;

class GOAPAction
{

protected:
	bool InRange;

	// Do Reset in the below section handles calling this function
	virtual void Reset() = 0;

public:
	// List of the conditions required for this action to be valid
	TMap<FString, bool> Preconditions;

	// The changes this action has on the world
	TMap<FString, bool> Effects;

	// The cost of the action
	float ActionCost;

	// The target the action works on
	AActor* Target;

	// Constructor
	GOAPAction();

	// Destructor
	~GOAPAction();

	// Reset the action to its base state
	void DoReset();

	// Adding preconditions
	void AddPrecondition(FString _name, bool _state);

	// Remove preconditions
	void RemovePrecondition(FString _name, bool _state);

	// Add effects
	void AddEffect(FString _name, bool _state);

	// Remove effect
	void RemoveEffect(FString _name, bool _state);

	// Sets and checks whether or not a
	bool IsInRange() const;

	// Sets the value of the range
	void SetInRange (bool _range);

	// Whether or not an action has finished executing
	virtual bool IsActionDone() = 0;

	// Pure virtual functions that the child classes must inherit
	virtual bool CheckProceduralPreconditions(AShip* _ship) = 0;

	// Performs the action
	virtual bool PerformAction(AShip* _ship, float _deltaTime) = 0;

	// Whether or not the action requires something in range
	virtual bool RequiresInRange() = 0;
public:
	
};
