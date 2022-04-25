/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Agents/Ship.h"

#include "Actions/CollectResourceAction.h"
#include "Actions/CollectRumAction.h"
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
	CurrentIdleTime = 0;
	CurrentIdleThreshold = MaxIdleTime;

	// Add in the collecting treasure action
	TreasureAction = new CollectTreasureAction();
	TreasureAction->AddPrecondition("HasMorale", false);
	TreasureAction->AddEffect("HasMorale", true);
	AvailableActions.Add(TreasureAction);

	// Create a final roam action for when resources are deposited
	CollectTreasureAction* finalRoamAction = new CollectTreasureAction();
	finalRoamAction->AddPrecondition("ResourcesDeposited", true);
	finalRoamAction->AddPrecondition("HasRum", true);
	finalRoamAction->AddPrecondition("Completed", false);
	finalRoamAction->AddEffect("Completed", true);
	AvailableActions.Add(finalRoamAction);

	// Add in a rum collection action for when rum is low
	RumAction = new CollectRumAction();
	RumAction->AddPrecondition("HasRum", false);
	RumAction->AddEffect("HasRum", true);
	AvailableActions.Add(RumAction);
	
	// Create the new state machine and register the states
	ActionStateMachine = new StateMachine<EAgentState, AShip>(this, NOTHING);
	ActionStateMachine->RegisterState(IDLE, &AShip::OnIdleEnter, &AShip::OnIdleTick, &AShip::OnIdleExit);
	ActionStateMachine->RegisterState(MOVE, &AShip::OnMoveEnter, &AShip::OnMoveTick, &AShip::OnMoveExit);
	ActionStateMachine->RegisterState(ACTION, &AShip::OnActionEnter, &AShip::OnActionTick, &AShip::OnActionExit);
	ActionStateMachine->RegisterState(BACKTRACK, &AShip::OnBacktrackEnter, &AShip::OnBacktrackTick, &AShip::OnBacktrackExit);
	ActionStateMachine->ChangeState(IDLE);
}


void AShip::SetResourceTarget(EGridType resourceTarget)
{
	// Update the resource
	ResourceType = resourceTarget;
	
	// Add in the collecting resource action
	CollectResourceAction* resourceAction = new CollectResourceAction(ResourceType);
	resourceAction->AddPrecondition("HasMorale", true);
	resourceAction->AddPrecondition("ResourcesDeposited", false);
	resourceAction->AddPrecondition("HasRum", true);
	resourceAction->AddPrecondition("CollectedResource", false);
	resourceAction->AddEffect("CollectedResource", true);
	AvailableActions.Add(resourceAction);

	// Add in the depositing resource action
	DepositResourceAction* depositAction = new DepositResourceAction(ResourceType, resourceAction->ResourceToGather);
	depositAction->AddPrecondition("HasMorale", true);
	depositAction->AddPrecondition("CollectedResource", true);
	resourceAction->AddPrecondition("ResourcesDeposited", false);
	depositAction->AddPrecondition("HasRum", true);
	depositAction->AddEffect("CollectedResource", false);
	depositAction->AddEffect("ResourcesDeposited", true);
	AvailableActions.Add(depositAction);

	// Set the rum level
	RumAction->Level = Level;
}


// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

	// Update the positions
	PrevXPos = XPos;
	PrevYPos = YPos;

	// Set up the amount of rum on the ship
	NumRum = MaxRum;
}


void AShip::OnIdleEnter()
{
	
}


void AShip::OnIdleTick(float deltaTime)
{
	MovementDirection = FVector::ZeroVector;

	// Check if the idle time is greater than the max
	if (CurrentIdleTime >= CurrentIdleThreshold)
	{
		// Reset the idle
		CurrentIdleTime = 0;

		// Reset the idle threshold
		CurrentIdleThreshold = FMath::RandRange(MinIdleTime, MaxIdleTime);

		// Wait some time and try again
		if (PathRetries >= PathRetriesMax)
		{
			LookForGold = true;
			PathRetries = 0;
			UE_LOG(LogTemp, Warning, TEXT("%s - Looking for gold now"), *GetName());
		}

		// In this state, we look to create a plan. Get the world state
		const TMap<FString, bool> worldState = GetWorldState();

		// Get the desired goal state
		const TMap<FString, bool> goalState = GetGoalState();

		// Check the world state for a completed system
		if (worldState["ResourcesDeposited"])
		{
			UE_LOG(LogTemp, Warning, TEXT("%s has completed all resourcing actions!"), *GetName());
			IsComplete = true;
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
	// If no morale left, mutiny!
	if (Morale <= 0)
		return;
	
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

			// Check if the final goal node is missing
			if (!willCollide)
				willCollide = !goalWorldNode || !goalWorldNode->ResourceAtLocation;

			// Check if any of the next [x] positions will have a collision
			for (int i = 0; i < 5; i ++)
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
					if (Level->UpdateAgentLocation(this, XPos, YPos, Path[0]->X, Path[0]->Y))
					{
						// Stores the previous positions
						PrevXPos = XPos;
						PrevYPos = YPos;
						
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

					// Otherwise if failed
					else
					{
						// Update to the previous position
						const float previousXPos = XPos * ALevelGenerator::GRID_SIZE_WORLD;
						const float previousYPos = YPos * ALevelGenerator::GRID_SIZE_WORLD;
						SetActorLocation(FVector(previousXPos, previousYPos, currentPosition.Z));
					}
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

void AShip::OnBacktrackEnter()
{
	
}


void AShip::OnBacktrackTick(float deltaTime)
{
	FVector currentPosition = GetActorLocation();
	const float targetXPos = PrevXPos * ALevelGenerator::GRID_SIZE_WORLD;
	const float targetYPos = PrevYPos * ALevelGenerator::GRID_SIZE_WORLD;
	const FVector targetPosition(targetXPos, targetYPos, currentPosition.Z);

	// Get the next direction to travel
	FVector direction = targetPosition - currentPosition;
	direction.Normalize();

	// Add the direction to the current position (multiplied by a small multiple)
	currentPosition += direction * deltaTime * MoveSpeed * 0.2;
	SetActorLocation(currentPosition);
			
	// Update the movement direction (to the reverse of it to look like its reversing)
	MovementDirection = currentPosition - PreviousPosition;
	if (!MovementDirection.IsZero())
		MovementDirection.Normalize();
	MovementDirection = -MovementDirection;
	PreviousPosition = currentPosition;

	// Check if the distance to the next position is close enough
	if (FVector::Dist(currentPosition, targetPosition) <= Tolerance)
	{
		// Update the agents location
		if (Level->UpdateAgentLocation(this, XPos, YPos, PrevXPos, PrevYPos))
		{
			// Update the current position and path
			XPos = PrevXPos;
			YPos = PrevYPos;
		}

		// Subtract a morale
		Morale--;
		
		// Change back to the idle state to rethink
		ActionStateMachine->ChangeState( IDLE);
	}
}


void AShip::OnBacktrackExit()
{
	
}


void AShip::HandleCollision(bool collider)
{
	// If already in collision mode, switch to idle
	if (GetAgentState() == BACKTRACK)
	{
		ActionStateMachine->ChangeState(IDLE);
		return;
	}

	// If in action mode, ignore the collision
	if (GetAgentState() == ACTION)
		return;
	
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
		// Change to the backtrack mode
		ActionStateMachine->ChangeState( BACKTRACK);
	}
}


TMap<FString, bool> AShip::GetWorldState()
{
	// Create a new empty state
	TMap<FString, bool> worldState;
	
	// Add in a morale flag
	worldState.Add("HasMorale", IsMoraleReached() && !LookForGold);

	// Add in a rum flag (or if a merchant)
	worldState.Add("HasRum", NumRum > RumThreshold || IsMerchant() || !Level->IsRumAvailable());

	// Add in resource collected flag
	worldState.Add("CollectedResource", GetResourceCollected() > 0);

	// Add in the final deposited world state
	worldState.Add("ResourcesDeposited", !Level->ResourcesExist(ResourceType));

	// The ship never really completes
	worldState.Add("Completed", false);

	// Returns the state
	return worldState;
}


TMap<FString, bool> AShip::GetGoalState()
{
	// Create a new empty state
	TMap<FString, bool> goalState;

	// Make sure the morale is valid
	goalState.Add("HasMorale", true);

	// The resources have all been deposited
	goalState.Add("ResourcesDeposited", true);

	// Store the completed state
	goalState.Add("Completed", true);

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
		case MERCHANT_RESOURCE:
			return NumMerchant;
		default:
			return 0;
	}
}


int AShip::GetResourcesRequired() const
{
	// This number is fixed
	return 50;
}


FString AShip::GetShipName() const
{
	FString name = "Missing";
	switch (ResourceType)
	{
		case FRUIT_RESOURCE:
			name = "Fruit"; break;
		case STONE_RESOURCE:
			name = "Stone"; break;
		case WOOD_RESOURCE:
			name = "Wood"; break;
		case MERCHANT_RESOURCE:
			name = "Merchant"; break;
		default:
			break;
	}

	// Return a formatted name
	return FString::Printf(TEXT("Ship #%02d: %s"), ShipNumber + 1, ToCStr(name));
}

EAgentState AShip::GetAgentState() const
{
	return ActionStateMachine->GetCurrentState();
}


bool AShip::IsMoraleReached() const
{
	// Morale is only low if no rum
	if (NumRum <= RumThreshold && !IsMerchant())
		return Morale > MissingRumMorale;

	// Otherwise check if morale is greater than the target
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

void AShip::DepositRum(int num)
{
	NumMerchant -= num;
	Level->TotalRumCollected += num;
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

bool AShip::IsMerchant() const
{
	return ResourceType == MERCHANT_RESOURCE;
}
