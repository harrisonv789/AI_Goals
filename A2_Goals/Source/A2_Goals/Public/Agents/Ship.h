/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "AgentState.h"
#include "GameFramework/Actor.h"
#include "World/LevelGenerator.h"
#include "Planning/StateMachine.h"
#include "Actions/GOAPAction.h"
#include "Ship.generated.h"

/**
 * @brief The base actor class for all ships that can navigate throughout the
 *		game and can follow the goal process.
 */
UCLASS(Blueprintable, BlueprintType)
class A2_GOALS_API AShip : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float Tolerance;

	// The current state machine of the ship
	StateMachine<EAgentState, AShip>* ActionStateMachine;

	// A list of all available actions
	TSet<GOAPAction*> AvailableActions;

	// A list of actions to execute
	TQueue<GOAPAction*> CurrentActions;

public:	
	// Sets default values for this actor's properties
	AShip();
	bool GeneratePath = true;
	bool FinishedMoving = false;
	TArray<GridNode*> Path;
	ALevelGenerator* Level;
	int Morale = 100;
	int TargetMorale = 100;

	// The direction that the current ship is moving towards
	UPROPERTY(BlueprintReadOnly)
	FVector MovementDirection;

	int XPos;
	int YPos;

	EGridType ResourceType;

	UPROPERTY()
	TArray<AActor*> PathDisplayActors;

	// The maximum time the ship can remain in an idle state
	float MaxIdleTime;

	// The current time the ship has remained idling
	float CurrentIdleTime;

	// Stores the number of each resource collected
	int NumStone;
	int NumWood;
	int NumFruit;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnIdleEnter ();
	void OnIdleTick (float _deltaTime);
	void OnIdleExit ();

	void OnMoveEnter ();
	void OnMoveTick (float _deltaTime);
	void OnMoveExit ();

	void OnActionEnter ();
	void OnActionTick (float _deltaTime);
	void OnActionExit ();

	/**
	 * @brief Determines the current state of the ship in the world
	 * @return A mapping of the new world state(s)
	 */
	TMap<FString, bool> GetWorldState ();

	/**
	 * @brief Determines the target goal state the agent should reach
	 * @return A mapping of the goal state(s)
	 */
	TMap<FString, bool> GetGoalState ();

	/**
	 * @brief A plan was unable to be found. Likely to idle for a bit
	 * @param _failedGoalState The goal state mapping
	 */
	void OnPlanFailed (TMap<FString, bool> _failedGoalState);

	/**
	 * @brief A plan was aborted midway through an action
	 * @param _failedAction The action that failed to execute
	 */
	void OnPlanAborted (GOAPAction* _failedAction);

public:	
	// Called every frame
	virtual void Tick(float _deltaTime) override;
};
