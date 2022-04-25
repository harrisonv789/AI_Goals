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
	TargetNode = nullptr;
	ResourceGathered = 0;
	ActionTime = 0.0;
}


bool CollectResourceAction::IsActionDone()
{
	// Return whether or not the treasure is complete
	const bool resourcesUsed = Target && Target->IsA(AResourceActor::StaticClass()) &&
		!Cast<AResourceActor>(Target)->HasResources();
	return ResourceGathered >= ResourceToGather || resourcesUsed;
}


bool CollectResourceAction::CheckProceduralPreconditions(AShip* ship)
{
	// Ensure the ship exists
	if (!ship)
		return false;

	// Calculate the nearest resource
	GridNode* goalNode = ship->Level->CalculateNearestGoal(ship->XPos, ship->YPos, ResourceType, true);

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


bool CollectResourceAction::PerformAction(AShip* ship, float deltaTime)
{
	// Add the current time to the count
	ActionTime += deltaTime;

	// Get the resource actor
	AResourceActor* resourceActor = Cast<AResourceActor>(Target);

	// Check if the resource is invalid
	if (!resourceActor)
		return false;

	// If the resources don't exist anymore
	if (!resourceActor->HasResources())
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
			case MERCHANT_RESOURCE:
				ship->NumMerchant++;
				break;

			// No default case
			default:
				break;
		}

		// Take a resource from the actor
		resourceActor->TakeResource();

		// Increase the current resource gathered
		ResourceGathered++;

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
			
			// Decrease the rum count
			if (!ship->IsMerchant())
				ship->NumRum--;
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
