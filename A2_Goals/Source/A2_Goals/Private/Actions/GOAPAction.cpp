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


void GOAPAction::AddPrecondition(FString name, bool state)
{
	Preconditions.Add(name, state);
}


void GOAPAction::RemovePrecondition(FString name)
{
	Preconditions.Remove(name);
}


void GOAPAction::AddEffect(FString name, bool state)
{
	Effects.Add(name, state);
}


void GOAPAction::RemoveEffect(FString name)
{
	Effects.Remove(name);
}


bool GOAPAction::IsInRange() const
{
	return InRange;
}


void GOAPAction::SetInRange(bool range)
{
	InRange = range;
}
