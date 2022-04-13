/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Agents/Ship.h"

#include "Actions/CollectResourceAction.h"
#include "Actions/CollectTreasureAction.h"
#include "Planning/GOAPPlanner.h"
#include "Actions/GOAPAction.h"

// Sets default values
AShip::AShip()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 100;
	Tolerance = 1;
	MovementDirection = FVector::ZeroVector;

	// Set the default idles
	MaxIdleTime = 3;
	CurrentIdleTime = 0;

	// Add in the collecting treasure action
	CollectTreasureAction* treasureAction = new CollectTreasureAction();
	treasureAction->AddPrecondition("HasMorale", false);
	treasureAction->AddEffect("HasMorale", true);
	AvailableActions.Add(treasureAction);

	// Add in the collecting resource action
	CollectResourceAction* resourceAction = new CollectResourceAction(ResourceType);
	resourceAction->AddPrecondition("HasMorale", true);
	resourceAction->AddPrecondition("HasResource", false);
	resourceAction->AddEffect("HasResource", true);
	//AvailableActions.Add(resourceAction);
	
	// Create the new state machine and register the states
	ActionStateMachine = new StateMachine<EAgentState, AShip>(this, NOTHING);
	ActionStateMachine->RegisterState(IDLE, &AShip::OnIdleEnter, &AShip::OnIdleTick, &AShip::OnIdleExit);
	ActionStateMachine->RegisterState(MOVE, &AShip::OnMoveEnter, &AShip::OnMoveTick, &AShip::OnMoveExit);
	ActionStateMachine->RegisterState(ACTION, &AShip::OnActionEnter, &AShip::OnActionTick, &AShip::OnActionExit);
	ActionStateMachine->ChangeState(IDLE);
}


// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
}


void AShip::OnIdleEnter()
{
	
}


void AShip::OnIdleTick(float deltaTime)
{
	FinishedMoving = true;
	MovementDirection = FVector::ZeroVector;

	// Check if the idle time is greater than the max
	if (CurrentIdleTime >= MaxIdleTime)
	{
		// Reset the idle
		CurrentIdleTime = 0;

		// In this state, we look to create a plan. Get the world state
		const TMap<FString, bool> worldState = GetWorldState();

		// Get the desired goal state
		const TMap<FString, bool> goalState = GetGoalState();

		// Attempt to make the plan and check success
		if (GOAPPlanner::Plan(this, AvailableActions, CurrentActions, worldState, goalState))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s has found a plan! Executing plan now..."), *GetName());
			ActionStateMachine->ChangeState(ACTION);
		}

		// If a plan failed
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s was unable to find a plan. Idling for %f seconds."), *GetName(), MaxIdleTime);
		}
	}

	// Otherwise increment the idle
	else
	{
		CurrentIdleTime += deltaTime;
	}
}


void AShip::OnIdleExit()
{
	
}


void AShip::OnMoveEnter()
{
	// Check to see if there are no actions and cancel the current moving
	if (CurrentActions.IsEmpty())
	{
		ActionStateMachine->ChangeState(IDLE);
		return;
	}

	// If the current action requires an 'in-range' check, AND the target is missing, return to planning
	GOAPAction* currentAction = *CurrentActions.Peek();
	if (currentAction->RequiresInRange() && currentAction->Target == nullptr)
	{
		ActionStateMachine->ChangeState(IDLE);
		return;
	}

	// If no fails, make a plan to move to the new action
	if (currentAction->RequiresInRange())
	{
		// Finds the node with the action's target
		GridNode* goalLocation = Level->FindGridNode(currentAction->Target);
		if (goalLocation)
		{
			// Creates a new path to the goal
			Level->CalculatePath(this, goalLocation);
		}
	}
}


void AShip::OnMoveTick(float deltaTime)
{
	// Get the current action that we are executing
	GOAPAction* currentAction = *CurrentActions.Peek();

	// If currently moving
	if (!FinishedMoving)
	{
		// If more grids to traverse
		if (Path.Num() > 0)
		{
			// Get the current location
			FVector currentPosition = GetActorLocation();

			// Calculates the target position based off the X & Y values inside of path
			const float targetXPos = Path[0]->X * ALevelGenerator::GRID_SIZE_WORLD;
			const float targetYPos = Path[0]->Y * ALevelGenerator::GRID_SIZE_WORLD;
			const FVector targetPosition(targetXPos, targetYPos, currentPosition.Z);

			// Get the next direction to travel
			FVector direction = targetPosition - currentPosition;
			direction.Normalize();

			// Update the movement direction
			MovementDirection = direction;

			// Add the direction to the current position
			currentPosition += direction * deltaTime * MoveSpeed;
			SetActorLocation(currentPosition);

			// Check if the distance to the next position is close enough
			if (FVector::Dist(currentPosition, targetPosition) <= Tolerance)
			{
				// Update the current position and path
				XPos = Path[0]->X;
				YPos = Path[0]->Y;
				currentPosition = targetPosition;
				Path.RemoveAt(0);
				Morale--;

				FinishedMoving = true;
			}
		}

		// Otherwise, if no more grids
		else
		{
			// Sets the current action to be in range as the location is now correct
			currentAction->SetInRange(true);

			// Reset the movement direction
			MovementDirection = FVector::ZeroVector;

			// Change to the action state to perform the next action
			ActionStateMachine->ChangeState(ACTION);
		}
	}
}


void AShip::OnMoveExit()
{
	
}


void AShip::OnActionEnter()
{
	
}


void AShip::OnActionTick(float deltaTime)
{
	// We do not need to move on this action
	FinishedMoving = true;

	// If we have no states remaining, we change to idle and exit
	if (CurrentActions.IsEmpty())
	{
		ActionStateMachine->ChangeState(IDLE);
		return;
	}

	// Check to see if our action has completed
	GOAPAction* currentAction = *CurrentActions.Peek();
	if (currentAction->IsActionDone())
	{
		// We have finished with the action, we can now remove
		CurrentActions.Dequeue(currentAction);

		// Delete all path actors once the goal is reached
		for (const auto pathObject : PathDisplayActors)
		{
			pathObject->Destroy();
		}

		// Remove the empty references
		PathDisplayActors.Empty();
	}

	// Check if there are still more actions to execute
	if (!CurrentActions.IsEmpty())
	{
		// Get to the top of the queue again
		currentAction = *CurrentActions.Peek();

		// Check to see if we need to be within range for an action
		// If no range requirement, return true
		const bool inRange = currentAction->RequiresInRange() ? currentAction->IsInRange() : true;

		// If we are in range, attempt the action
		if (inRange)
		{
			// Attempt to perform the action
			const bool isActionSuccessful = currentAction->PerformAction(this, deltaTime);

			// If we fail the action, change to the IDLE state and report that we abort
			if (!isActionSuccessful)
			{
				ActionStateMachine->ChangeState(IDLE);
				OnPlanAborted(currentAction);
			}
		}

		// If not in range
		else
		{
			// At this point, we have a valid action but we are not in range
			ActionStateMachine->ChangeState(MOVE);
		}
	}

	// If actions are empty
	else
	{
		// No actions remaining, return to idle state
		ActionStateMachine->ChangeState(IDLE);
	}
}


void AShip::OnActionExit()
{
	
}


TMap<FString, bool> AShip::GetWorldState()
{
	// Create a new empty state
	TMap<FString, bool> worldState;

	// Add in a morale flag
	worldState.Add("HasMorale", IsMoraleReached());

	// TODO: Update with the actual resource
	//worldState.Add("HasResource", false);

	// Returns the state
	return worldState;
}


TMap<FString, bool> AShip::GetGoalState()
{
	// Create a new empty state
	TMap<FString, bool> goalState;

	// Make sure the morale is valid
	goalState.Add("HasMorale", true);

	// TODO: Update with the real goal
	//goalState.Add("HasResource", true);

	// Returns the state
	return goalState;
}


void AShip::OnPlanFailed(TMap<FString, bool> failedGoalState)
{
	
}


void AShip::OnPlanAborted(GOAPAction* failedAction)
{
	
}


// Called every frame
void AShip::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// Tick the state machine
	ActionStateMachine->Tick(deltaTime);
}


EGridType AShip::GetResourceType() const
{
	return ResourceType;
}


int AShip::GetResourceCollected() const
{
	switch (ResourceType)
	{
		case FRUIT_RESOURCE:
			return NumFruit;
		case STONE_RESOURCE:
			return NumStone;
		case WOOD_RESOURCE:
			return NumWood;
		default:
			return 0;
	}
}


int AShip::GetResourcesRequired() const
{
	// TODO: Fix this number
	return 50;
}


FString AShip::GetShipName() const
{
	FString merchantName = "Missing";
	switch (ResourceType)
	{
		case FRUIT_RESOURCE:
			merchantName = "Fruit"; break;
		case STONE_RESOURCE:
			merchantName = "Stone"; break;
		case WOOD_RESOURCE:
			merchantName = "Wood"; break;
		default:
			break;
	}

	// Return a formatted name
	return FString::Printf(TEXT("Ship #%02d : %s Merchant"), ShipNumber + 1, ToCStr(merchantName));
}


bool AShip::IsMoraleReached() const
{
	return Morale > TargetMorale;
}
