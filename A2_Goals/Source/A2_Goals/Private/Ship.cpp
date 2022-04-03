/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Ship.h"
#include "Planning/GOAPPlanner.h"
#include "TestAction.h"


// Sets default values
AShip::AShip()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

	// Create the reload action
	TestAction reloadAction;
	reloadAction.AddPrecondition(TEXT("CanShoot"), true);
	reloadAction.AddPrecondition(TEXT("HasAmmo"), false);
	reloadAction.AddEffect(TEXT("HasAmmo"), true);

	// Create the shoot action
	TestAction shootActionState;
	shootActionState.AddPrecondition(TEXT("TargetDead"), false);
	shootActionState.AddPrecondition(TEXT("CanShoot"), true);
	shootActionState.AddPrecondition(TEXT("HasAmmo"), true);
	shootActionState.AddEffect(TEXT("TargetDead"), true);

	// Create another action
	TestAction testAction3;
	testAction3.AddPrecondition(TEXT("NearDeath"), true);
	testAction3.AddEffect(TEXT("NearDeath"), false);

	// Create a list of available actions
	TSet<GOAPAction*> newAvailableActions;
	newAvailableActions.Add(&reloadAction);
	newAvailableActions.Add(&shootActionState);
	newAvailableActions.Add(&testAction3);

	// Create a queue of actions
	TQueue<GOAPAction*> plannedActions;

	// Create the start state
	TMap<FString, bool> startState;
	startState.Add(TEXT("TargetDead"), false);
	startState.Add(TEXT("HasAmmo"), false);
	startState.Add(TEXT("CanShoot"), true);
	startState.Add(TEXT("NearDeath"), false);
	
	// Create the goal state
	TMap<FString, bool> goalState;
	goalState.Add(TEXT("TargetDead"), true);

	// Create a plan
	// TODO Refactor and replace this
	if (GOAPPlanner::Plan(
		this, newAvailableActions, plannedActions, startState, goalState))
	{
		UE_LOG(LogTemp, Warning, TEXT("Plan Found"));

		GOAPAction* currentAction;
		while (plannedActions.Dequeue(currentAction))
		{
			for (auto condition : currentAction->Preconditions)
			{
				UE_LOG(LogTemp, Warning, TEXT("Preconditions: %s Value: %d"),
					*condition.Key, condition.Value);
			}

			for (auto effect : currentAction->Effects)
			{
				UE_LOG(LogTemp, Warning, TEXT("Effect: %s Value: %d"),
					*effect.Key, effect.Value);
			}
		}
	}

	// No plan has been found
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Plan Found"));
	}
	
}


// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

