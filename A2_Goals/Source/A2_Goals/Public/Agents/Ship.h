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
#include "World/GridType.h"
#include "Ship.generated.h"

/**
 * @brief The base actor class for all ships that can navigate throughout the
 *		game and can follow the goal process.
 */
UCLASS(Blueprintable, BlueprintType)
class A2_GOALS_API AShip : public AActor
{
	GENERATED_BODY()

	/************************************************************/
	protected:

	// The speed of movement
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MoveSpeed;

	// The distance to the goal the ship should be before it is reached
	UPROPERTY(EditAnywhere, Category = "Stats")
	float Tolerance;

	// The current state machine of the ship
	StateMachine<EAgentState, AShip>* ActionStateMachine;

	// A list of all available actions
	TSet<GOAPAction*> AvailableActions;

	// A list of actions to execute
	TQueue<GOAPAction*> CurrentActions;

	
	/************************************************************/
	public:	

	// A flag for if the path is to be generated
	bool GeneratePath = true;

	// A flag for whether movement is complete
	bool FinishedMoving = false;

	// A list of grid nodes for the ship's path
	TArray<GridNode*> Path;

	// The current Level Generator
	UPROPERTY()
	ALevelGenerator* Level;

	// The ship number
	UPROPERTY(BlueprintReadOnly)
	int ShipNumber;

	// The current ships Morale
	UPROPERTY(BlueprintReadOnly)
	int Morale = 100;

	// The ships required Morale
	UPROPERTY(BlueprintReadOnly)
	int TargetMorale = 100;

	// The direction that the current ship is moving towards
	UPROPERTY(BlueprintReadOnly)
	FVector MovementDirection;

	// The current position of the ship
	UPROPERTY(BlueprintReadOnly)
	int XPos;

	// The current Y position of the ship
	UPROPERTY(BlueprintReadOnly)
	int YPos;

	// The current resource the ship is searching for
	EGridType ResourceType;

	UPROPERTY()
	TArray<AActor*> PathDisplayActors;

	// The maximum time the ship can remain in an idle state
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxIdleTime;

	// The current time the ship has remained idling
	float CurrentIdleTime;

	// Stores the number of each stone collected
	UPROPERTY(BlueprintReadOnly)
	int NumStone;

	// Stores the number of each wood collected
	UPROPERTY(BlueprintReadOnly)
	int NumWood;
	
	// Stores the number of each fruit collected
	UPROPERTY(BlueprintReadOnly)
	int NumFruit;

	// Stores the number of gold collected
	UPROPERTY(BlueprintReadOnly)
	int NumGold;

	
	/************************************************************/
	protected:
	
	/**
	 * @brief Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Called when the IDLE state is entered
	 */
	void OnIdleEnter ();

	/**
	 * @brief Called when the IDLE state is ticked
	 * @param deltaTime The time-step [s]
	 */
	void OnIdleTick (float deltaTime);

	/**
	 * @brief Called when the IDLE state is exited
	 */
	void OnIdleExit ();

	/**
	 * @brief Called when the MOVE state is entered
	 */
	void OnMoveEnter ();

	/**
	 * @brief Called when the MOVE state is ticked
	 * @param deltaTime The time-step [s]
	 */
	void OnMoveTick (float deltaTime);

	/**
	 * @brief Called when the MOVE state is exited
	 */
	void OnMoveExit ();

	/**
	 * @brief Called when the ACTION state is entered
	 */
	void OnActionEnter ();
	
	/**
	 * @brief Called when the ACTION state is ticked
	 * @param deltaTime The time-step [s]
	 */
	void OnActionTick (float deltaTime);

	/**
	 * @brief Called when the ACTION state is exited
	 */
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
	 * @param failedGoalState The goal state mapping
	 */
	void OnPlanFailed (TMap<FString, bool> failedGoalState);

	/**
	 * @brief A plan was aborted midway through an action
	 * @param failedAction The action that failed to execute
	 */
	void OnPlanAborted (GOAPAction* failedAction);

	
	/************************************************************/
	public:
	
	/**
	 * @brief Default constructor
	 */
	AShip();

	/**
	 * @brief Updates the Resource Target and creates an action to reach
	 * @param resourceTarget The new resource type
	 */
	void SetResourceTarget (EGridType resourceTarget);

	/**
	 * @brief Called every frame
	 * @param deltaTime The time-step [s]
	 */
	virtual void Tick(float deltaTime) override;

	/**
	 * @brief Returns the current resource type the ship looks for
	 * @return The Resource Type the ship is looking for
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	EGridType GetResourceType () const;
	
	/**
	 * @brief Returns the number of resources currently collected
	 * @return The number of resources collected
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	int GetResourceCollected () const;
	
	/**
	 * @brief Returns the number of resources acquired
	 * @return The number of resources required
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	int GetResourcesRequired () const;

	/**
	 * @brief Returns the name of the ship
	 * @return The name of the ship
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetShipName () const;

	/**
	 * @brief Returns whether or not the morale has been reached
	 * @return A flag for whether or not the morale is reached
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMoraleReached () const;
};
