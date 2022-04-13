/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Actions/CollectTreasureAction.h"

#include "Agents/Ship.h"
#include "World/GridNode.h"

CollectTreasureAction::CollectTreasureAction()
{
	// Reset the action
	Reset();
}


CollectTreasureAction::~CollectTreasureAction()
{
	
}


void CollectTreasureAction::Reset()
{
	// Ensure the in range is false
	SetInRange(false);
	Target = nullptr;
	TreasureGathered = 0;
	ActionTime = 0.0;
}


bool CollectTreasureAction::IsActionDone()
{
	// Return whether or not the treasure is complete
	return TreasureGathered >= TreasureToGather;
}


bool CollectTreasureAction::CheckProceduralPreconditions(AShip* ship)
{
	// If the ship does not exist
	if (!ship)
	{
		return false;
	}

	// Calculate the nearest gold node
	GridNode* goalNode = ship->Level->CalculateNearestGoal(ship->XPos, ship->YPos, EGridType::GOLD_RESOURCE);

	// Check if the resource exiss
	if (!goalNode || !goalNode->ResourceAtLocation)
		return false;

	// Get the target
	Target = goalNode->ResourceAtLocation;

	// Get the distance to the target
	FVector distance = ship->GetActorLocation() - Target->GetActorLocation();

	// Check if the distance is less than some amount
	SetInRange(distance.Size() <= 5);

	// Return a success
	return true;
}


bool CollectTreasureAction::PerformAction(AShip* ship, float deltaTime)
{
	// Add the current time to the count
	ActionTime += deltaTime;

	// Get the gold resource
	AGold* goldResource = Cast<AGold>(Target);

	// Check if the gold is invalid
	if (!goldResource || !ship->Level->IsGoldValid(goldResource))
		return false;

	// Check the action time
	if (ActionTime >= TimeToCollect)
	{
		// Increase the morale
		TreasureGathered++;
		ship->Morale = 200;
		ship->Level->CollectGold(goldResource);

		// Remove the target
		Target = nullptr;
	}

	// Return a success
	return true;
}


bool CollectTreasureAction::RequiresInRange()
{
	// This must be next to or on top of the agent
	return true;
}
