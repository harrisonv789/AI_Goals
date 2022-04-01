#include "GOAPAction.h"

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

	// Call the child version
	Reset();
}

void GOAPAction::AddPrecondition(FString _name, bool _state)
{
	Preconditions.Add(_name, _state);
}

void GOAPAction::RemovePrecondition(FString _name, bool _state)
{
	Preconditions.Remove(_name);
}

void GOAPAction::AddEffect(FString _name, bool _state)
{
	Effects.Add(_name, _state);
}

void GOAPAction::RemoveEffect(FString _name, bool _state)
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
