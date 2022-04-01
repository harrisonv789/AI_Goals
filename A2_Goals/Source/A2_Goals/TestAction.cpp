#include "TestAction.h"

TestAction::TestAction()
{
}

TestAction::~TestAction()
{
}

void TestAction::Reset()
{
	// TODO
}

bool TestAction::IsActionDone()
{
	// TODO
	return true;
}

bool TestAction::CheckProceduralPreconditions(AShip* _ship)
{
	for (auto& entry : Preconditions)
	{
		UE_LOG(LogTemp, Warning, TEXT("Precondition Name: %s"), *entry.Key);
	}

	return true;
}

bool TestAction::PerformAction(AShip* _ship, float _deltaTime)
{
	// TODO
	return true;
}

bool TestAction::RequiresInRange()
{
	// TODO
	return true;
}
