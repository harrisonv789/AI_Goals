/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Agents/Ship.h"

#include "Actions/CollectResourceAction.h"
#include "Actions/CollectTreasureAction.h"
#include "Actions/DepositResourceAction.h"
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
	TreasureAction = new CollectTreasureAction();
	TreasureAction->AddPrecondition("HasMorale", false);
	TreasureAction->AddEffect("HasMorale", true);
	AvailableActions.Add(TreasureAction);
	
	// Create the new state machine and register the states
	ActionStateMachine = new StateMachine<EAgentState, AShip>(this, NOTHING);
	ActionStateMachine->RegisterState(IDLE, &AShip::OnIdleEnter, &AShip::OnIdleTick, &AShip::OnIdleExit);
	ActionStateMachine->RegisterState(MOVE, &AShip::OnMoveEnter, &AShip::OnMoveTick, &AShip::OnMoveExit);
	ActionStateMachine->RegisterState(ACTION, &AShip::OnActionEnter, &AShip::OnActionTick, &AShip::OnActionExit);
	ActionStateMachine->RegisterState(COMPLETE, &AShip::OnCompleteEnter, &AShip::OnCompleteTick, &AShip::OnCompleteExit);
	ActionStateMachine->ChangeState(IDLE);
}


void AShip::SetResourceTarget(EGridType resourceTarget)
{
	// Update the resource
	ResourceType = resourceTarget;
	
	// Add in the collecting resource action
	CollectResourceAction* resourceAction = new CollectResourceAction(ResourceType);
	resourceAction->AddPrecondition("HasMorale", true);
	resourceAction->AddPrecondition("CollectedResource", false);
	resourceAction->AddEffect("CollectedResource", true);
	AvailableActions.Add(resourceAction);

	// Add in the depositing resource action
	DepositResourceAction* depositAction = new DepositResourceAction(ResourceType, resourceAction->ResourceToGather);
	depositAction->AddPrecondition("HasMorale", true);
	depositAction->AddPrecondition("CollectedResource", true);
	depositAction->AddEffect("CollectedResource", false);
	depositAction->AddEffect("ResourcesDeposited", true);
	AvailableActions.Add(depositAction);
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
	MovementDirection = FVector::ZeroVector;

	// Check if the idle time is greater than the max
	if (CurrentIdleTime >= MaxIdleTime)
	{
		// Reset the idle
		CurrentIdleTime = 0;

		// Wait some time and try again
		if (PathRetries >= PathRetriesMax)
		{
			LookForGold = true;
			PathRetries = 0;
			UE_LOG(LogTemp, Warning, TEXT("%s - Looking for gold now"), *GetName());
		}

		// In this state, we look to create a plan. Get the world state
		TMap<FString, bool> worldState = GetWorldState();

		// Get the desired goal state
		TMap<FString, bool> goalState = GetGoalState();

		// Check if completed
		if (GOAPPlanner::CheckConditionsInState(goalState, worldState))
		{
			// Finished the mission
			ActionStateMachine->ChangeState(COMPLETE);

			return;
		}

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
			PathRetries += 1;
		}
	}

	// Otherwise increment the idle
	else
	{
		CurrentIdleTime += deltaTime;
	}
}


bool AShip::PlanPath(AActor* target)
{
	// Reset the goal
	GoalNode = nullptr;
	
	// Finds the node with the action's target
	GridNode* goalLocation = Level->FindGridNode(target);
	if (goalLocation)
	{
		// Creates a new path to the goal
		Level->CalculatePath(this, goalLocation);
		GoalNode = goalLocation;
		
	} else
		return false;

	// Returns a success from the path
	return !GeneratePath;
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
		UE_LOG(LogTemp, Warning, TEXT("%s - Actions are Empty"), *GetName());
		return;
	}

	// If the current action requires an 'in-range' check, AND the target is missing, return to planning
	GOAPAction* currentAction = *CurrentActions.Peek();
	if (currentAction->RequiresInRange() && currentAction->Target == nullptr)
	{
		ActionStateMachine->ChangeState(IDLE);
		UE_LOG(LogTemp, Warning, TEXT("%s - Missing Target"), *GetName());
		return;
	}

	// If no fails, make a plan to move to the new action
	if (currentAction->RequiresInRange())
	{
		// Attempt to path plan and if it fails, change to idle
		if (!PlanPath(currentAction->Target))
		{
			FinishedMoving = true;
			UE_LOG(LogTemp, Warning, TEXT("Failed to find a path for %s"), *GetName());

			// Update the path retries
			PathRetries += 1;

			// Change to the Idle
			ActionStateMachine->ChangeState(IDLE);

			return;
		}
	}

	// Reset the retries
	PathRetries = 0;

	// Ensure the ships can start moving
	FinishedMoving = false;
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
			// Get the current node at the world
			const GridNode* goalWorldNode = currentAction->TargetNode;

			// A flag for if a collision will occur
			bool willCollide = false;
			
			// Check if the final goal node has a finished agent
			if (!willCollide)
				willCollide = goalWorldNode->AgentAtLocation != nullptr && Cast<AShip>(goalWorldNode->AgentAtLocation)->GoalNode == goalWorldNode;

			// Check if any of the next [x] positions will have a collision
			for (int i = 0; i < 4; i ++)
			{
				const GridNode* nextGridNode = Path.Num() > i ? Level->WorldArray[Path[i]->X][Path[i]->Y] : nullptr;
				if (!willCollide && nextGridNode)
					willCollide = nextGridNode->AgentAtLocation != nullptr && nextGridNode->AgentAtLocation != this;
			}
			
			// If a collision occurred
			if (willCollide)
			{
				// Handle a collision
				HandleCollision();
				
				// Return from this tick
				return;
			}
			
			// Get the current location
			FVector currentPosition = GetActorLocation();

			// Calculates the target position based off the X & Y values inside of path
			const float targetXPos = Path[0]->X * ALevelGenerator::GRID_SIZE_WORLD;
			const float targetYPos = Path[0]->Y * ALevelGenerator::GRID_SIZE_WORLD;
			const FVector targetPosition(targetXPos, targetYPos, currentPosition.Z);

			// Get the next direction to travel
			FVector direction = targetPosition - currentPosition;
			direction.Normalize();

			// Add the direction to the current position
			currentPosition += direction * deltaTime * MoveSpeed;
			SetActorLocation(currentPosition);

			// Update the movement direction
			MovementDirection = currentPosition - PreviousPosition;
			if (!MovementDirection.IsZero())
				MovementDirection.Normalize();
			PreviousPosition = currentPosition;

			// Check if the distance to the next position is close enough
			if (FVector::Dist(currentPosition, targetPosition) <= Tolerance)
			{
				// Ensure path still exists
				if (Path.Num() > 0)
				{
					// Update the agent's location on the grid node
					Level->UpdateAgentLocation(this, XPos, YPos, Path[0]->X, Path[0]->Y);
					
					// Update the current position and path
					XPos = Path[0]->X;
					YPos = Path[0]->Y;

					// Set the current position to the exact target position
					currentPosition = targetPosition;
					SetActorLocation(currentPosition);

					// Remove the path and actors that exist here
					// Sometimes a collision will be triggered by the actor and remove a path
					if (Path.Num() > 0)
					{
						Path.RemoveAt(0);
						if (PathDisplayActors.Num() > 0)
							PathDisplayActors.Pop()->Destroy();
					}

					// Subtract a morale
					Morale--;
				}
			}
		}

		// Otherwise, if no more grids
		else
		{
			// Sets the current action to be in range as the location is now correct
			currentAction->SetInRange(true);

			// Reset the movement direction
			MovementDirection = FVector::ZeroVector;
			
			// Reset the goal path
			GoalNode = nullptr;

			// Finished moving
			FinishedMoving = true;

			// Change to the action state to perform the next action
			ActionStateMachine->ChangeState(ACTION);
		}
	}
}


void AShip::OnMoveExit()
{
	// Delete all path actors once the goal is reached
	for (const auto pathObject : PathDisplayActors)
		pathObject->Destroy();

	// Remove the empty references
	PathDisplayActors.Empty();
	Path.Empty();
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
		UE_LOG(LogTemp, Warning, TEXT("%s - Actions are empty"), *GetName());
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
		Path.Empty();
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
				UE_LOG(LogTemp, Warning, TEXT("%s - Plan Aborted"), *GetName());
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
		UE_LOG(LogTemp, Warning, TEXT("%s - Actions are empty"), *GetName());
	}
}


void AShip::OnActionExit()
{
	
}

void AShip::OnCompleteEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s - Actions are completed!"), *GetName());
}

void AShip::OnCompleteTick(float deltaTime)
{
}

void AShip::OnCompleteExit()
{
}


void AShip::HandleCollision(bool collider)
{
	// Delete all path actors once the goal is reached
	for (const auto pathObject : PathDisplayActors)
		pathObject->Destroy();

	// Remove the empty references
	PathDisplayActors.Empty();
	Path.Empty();

	// Log error on change
	UE_LOG(LogTemp, Warning, TEXT("%s found another agent in the way. Planning another route."), *GetName());

	// If detected a future collision
	if (!collider)
	{
		// Return the changing back to path planning
		ActionStateMachine->ChangeState(MOVE);
	}

	// If a collider got hit, stop
	else
	{
		// Return the changing back to goal planning
		ActionStateMachine->ChangeState(IDLE);
	}
}


TMap<FString, bool> AShip::GetWorldState()
{
	// Create a new empty state
	TMap<FString, bool> worldState;

	// Add in a morale flag
	worldState.Add("HasMorale", IsMoraleReached() && !LookForGold);

	// Add in resource collected flag
	worldState.Add("CollectedResource", GetResourceCollected() > 0);

	// Add in the final deposited world state
	worldState.Add("ResourcesDeposited", !Level->ResourcesExist(ResourceType));

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
	goalState.Add("ResourcesDeposited", true);

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
	return FString::Printf(TEXT("Ship #%02d: %s"), ShipNumber + 1, ToCStr(merchantName));
}

EAgentState AShip::GetAgentState() const
{
	return ActionStateMachine->GetCurrentState();
}


bool AShip::IsMoraleReached() const
{
	return Morale > TargetMorale;
}

void AShip::DepositFruit(int num)
{
	NumFruit -= num;
	Level->TotalFruitCollected += num;
}

void AShip::DepositStone(int num)
{
	NumStone -= num;
	Level->TotalStoneCollected += num;
}

void AShip::DepositWood(int num)
{
	NumWood -= num;
	Level->TotalWoodCollected += num;
}

void AShip::NotifyActorBeginOverlap(AActor* otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	// Check if it a gold
	if (otherActor->IsA(AGold::StaticClass()))
	{
		// Ensure the treasure from being in range
		TreasureAction->SetInRange(true);
	}
	
	// If it is another ship
	else if (otherActor->IsA(AShip::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ship has overlapped a collision!"));
		HandleCollision(true);
	}
}

void AShip::NotifyActorEndOverlap(AActor* otherActor)
{
	Super::NotifyActorEndOverlap(otherActor);

	// Check if it a gold
	if (otherActor->IsA(AGold::StaticClass()))
	{
		// Prevent the treasure from being in range
		TreasureAction->SetInRange(false);
	}
}

void AShip::CollectGold(AGold* gold)
{
	Morale = 200;
	NumGold++;
	Level->CollectGold(gold);
	LookForGold = false;
}

void AShip::Track()
{
	Level->TrackAgent(this);
	IsTracked = true;
}
