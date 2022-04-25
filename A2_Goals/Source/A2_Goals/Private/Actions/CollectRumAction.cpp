/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Actions/CollectRumAction.h"

#include "Agents/Ship.h"
#include "World/GridNode.h"

CollectRumAction::CollectRumAction()
{
	// Reset the action
	CollectRumAction::Reset();
}


CollectRumAction::~CollectRumAction()
{
	// Nothing currently
}


void CollectRumAction::Reset()
{
	// Ensure the in range is false
	SetInRange(false);
	Target = nullptr;
	TargetNode = nullptr;
	RumGathered = 0;
	ActionTime = 0.0;
}


bool CollectRumAction::IsActionDone()
{
	// Return whether or not the treasure is complete
	return RumGathered >= RumToCollect || !Level->IsRumAvailable();
}


bool CollectRumAction::CheckProceduralPreconditions(AShip* ship)
{
	// Ensure the ship exists
	if (!ship)
		return false;

	// Calculate the nearest resource
	GridNode* goalNode = ship->Level->CalculateNearestGoal(ship->XPos, ship->YPos, HOME, true);

	// Check if the resource exists
	if (!goalNode || !goalNode->ResourceAtLocation)
		return false;

	// Get the target
	Target = goalNode->ResourceAtLocation;
	TargetNode = goalNode;

	// Get the distance to the target
	const FVector distance = ship->GetActorLocation() - Target->GetActorLocation();

	// Check if the distance is less than some amount
	SetInRange(distance.Size() <= 5);

	// Return a success
	return true;
}


bool CollectRumAction::PerformAction(AShip* ship, float deltaTime)
{
	// Add the current time to the count
	ActionTime += deltaTime;

	// If the rum don't exist anymore
	if (!Level->IsRumAvailable())
	{
		// Remove the target
		Target = nullptr;
		TargetNode = nullptr;
			
		// Decrease the morale
		ship->Morale--;

		// Say done
		return true;
	}

	// Check the action time
	if (ActionTime >= TimeToCollect)
	{
		// Increase the amount of rum on the ship
		ship->NumRum++;

		// Take rum from the level
		Level->TakeRum();

		// Increase the rum of this action
		RumGathered++;

		// Reset the time
		ActionTime = 0.0;

		// If action is done, reset target and morale
		if (IsActionDone())
		{
			// Remove the target
			Target = nullptr;
			TargetNode = nullptr;
			
			// Decrease the morale
			ship->Morale--;
		}
	}

	// Return a success
	return true;
}


bool CollectRumAction::RequiresInRange()
{
	// This must be next to or on top of the agent
	return true;
}
