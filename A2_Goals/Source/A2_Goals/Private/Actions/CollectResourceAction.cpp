/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Actions/CollectResourceAction.h"

#include "Agents/Ship.h"
#include "Items/ResourceActor.h"
#include "World/GridNode.h"

CollectResourceAction::CollectResourceAction(EGridType resourceType)
{
	// Reset the action
	CollectResourceAction::Reset();

	// Updates the resource type
	SetResourceType(resourceType);
}


CollectResourceAction::~CollectResourceAction()
{
	// Nothing currently
}


void CollectResourceAction::Reset()
{
	// Ensure the in range is false
	SetInRange(false);
	Target = nullptr;
	ResourceGathered = 0;
	ActionTime = 0.0;
}


bool CollectResourceAction::IsActionDone()
{
	// Return whether or not the treasure is complete
	return ResourceGathered >= ResourceToGather;
}


bool CollectResourceAction::CheckProceduralPreconditions(AShip* ship)
{
	// Ensure the ship exists
	if (!ship)
		return false;

	// Calculate the nearest resource
	GridNode* goalNode = ship->Level->CalculateNearestGoal(ship->XPos, ship->YPos, ResourceType);

	// Check if the resource exists
	if (!goalNode || !goalNode->ResourceAtLocation)
		return false;

	// Get the target
	Target = goalNode->ResourceAtLocation;

	// Get the distance to the target
	const FVector distance = ship->GetActorLocation() - Target->GetActorLocation();

	// Check if the distance is less than some amount
	// TODO update with some actual collision system
	SetInRange(distance.Size() <= 5);

	// Return a success
	return true;
}


bool CollectResourceAction::PerformAction(AShip* ship, float deltaTime)
{
	// Add the current time to the count
	ActionTime += deltaTime;

	// Get the resource actor
	AResourceActor* resourceActor = Cast<AResourceActor>(Target);

	// Check if the resource is invalid
	if (!resourceActor)
		return false;

	// Check the action time
	if (ActionTime >= TimeToCollect)
	{
		// Increase the resource gathered based on the type
		switch (ResourceType)
		{
			case WOOD_RESOURCE:
				ship->NumWood++;
				break;
			case STONE_RESOURCE:
				ship->NumStone++;
				break;
			case FRUIT_RESOURCE:
				ship->NumFruit++;
				break;

			// No default case
			default:
				break;
		}

		// Increase the current resource gathered
		ResourceGathered++;

		// Reset the time
		ActionTime = 0.0;

		// If action is done, reset target and morale
		if (IsActionDone())
		{
			// Remove the target
			Target = nullptr;
			
			// Decrease the morale
			ship->Morale--;
		}
	}

	// Return a success
	return true;
}


bool CollectResourceAction::RequiresInRange()
{
	// This must be next to or on top of the agent
	return true;
}


void CollectResourceAction::SetResourceType(EGridType resourceType)
{
	ResourceType = resourceType;
}
