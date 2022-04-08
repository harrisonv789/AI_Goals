/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Actions/GOAPAction.h"

GOAPAction::GOAPAction()
{
	// Base implementation does nothing
}


GOAPAction::~GOAPAction()
{
	// Base implementation does nothing
}


void GOAPAction::DoReset()
{
	// Reset required variables
	InRange = false;
	Target = nullptr;

	// Call the subclass version
	Reset();
}


void GOAPAction::AddPrecondition(FString _name, bool _state)
{
	Preconditions.Add(_name, _state);
}


void GOAPAction::RemovePrecondition(FString _name)
{
	Preconditions.Remove(_name);
}


void GOAPAction::AddEffect(FString _name, bool _state)
{
	Effects.Add(_name, _state);
}


void GOAPAction::RemoveEffect(FString _name)
{
	Effects.Remove(_name);
}


bool GOAPAction::IsInRange() const
{
	return InRange;
}


void GOAPAction::SetInRange(bool _range)
{
	InRange = _range;
}
