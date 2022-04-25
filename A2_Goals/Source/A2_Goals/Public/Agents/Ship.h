/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "AgentState.h"
#include "Actions/CollectRumAction.h"
#include "Actions/CollectTreasureAction.h"
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

	// A reference to the collecting treasure action to update the in range
	CollectTreasureAction* TreasureAction;

	// A reference to the collecting rum action
	CollectRumAction* RumAction;

	// The previous position of the previous frame
	FVector PreviousPosition;

	// The number of retries this ship has attempted
	int PathRetries = 0;

	// The max number of retries a ship can try
	int PathRetriesMax = 2;

	// The maximum amount of rum a ship can carry
	UPROPERTY(EditAnywhere, Category = "Stats")
	int MaxRum = 75;

	// The threshold before the ship needs to look for rum
	UPROPERTY(EditAnywhere, Category = "Stats")
	int RumThreshold = 1;

	// The previous X position before the current one
	int PrevXPos;

	// The previous Y position before the current one
	int PrevYPos;

	
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

	// The ships lowered Morale if no rum
	UPROPERTY(BlueprintReadOnly)
	int MissingRumMorale = 40;

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

	// The minimum time the ship can remain in an idle state
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MinIdleTime = 1;

	// The maximum time the ship can remain in an idle state
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxIdleTime = 2;

	// The current time the ship has remained idling
	float CurrentIdleTime;

	// The current idle waiting time
	float CurrentIdleThreshold;

	// Stores the number of each stone collected
	UPROPERTY(BlueprintReadOnly)
	int NumStone;

	// Stores the number of each wood collected
	UPROPERTY(BlueprintReadOnly)
	int NumWood;
	
	// Stores the number of each fruit collected
	UPROPERTY(BlueprintReadOnly)
	int NumFruit;

	// Stores the number of each rum barrels collected
	UPROPERTY(BlueprintReadOnly)
	int NumMerchant;

	// Stores the number of gold collected
	UPROPERTY(BlueprintReadOnly)
	int NumGold;
	
	// Whether or not to look for gold (randomly)
	bool LookForGold = false;

	// The final node to target
	GridNode* GoalNode;

	// Sets whether or not this ship is currently being tracked
	UPROPERTY(BlueprintReadOnly)
	bool IsTracked = false;

	// A flag for if all tasks have been completed
	UPROPERTY(BlueprintReadOnly)
	bool IsComplete = false;

	// The amount of rum a ship has
	UPROPERTY(BlueprintReadOnly)
	int NumRum;

	
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
	 * @brief Called when the BACKTRACK state is entered
	 */
	void OnBacktrackEnter ();
	
	/**
	 * @brief Called when the BACKTRACK state is ticked
	 * @param deltaTime The time-step [s]
	 */
	void OnBacktrackTick (float deltaTime);

	/**
	 * @brief Called when the BACKTRACK state is exited
	 */
	void OnBacktrackExit ();

	/**
	 * @brief Handles collision of two agents
	 * @param collider Whether a collider has been hit - switch to idle
	 */
	void HandleCollision(bool collider = false);

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

	/**
	 * @brief Called when an actor overlaps another actor
	 * @param otherActor The actor that is being overlapped
	 */
	virtual void NotifyActorBeginOverlap(AActor* otherActor) override;

	/**
	 * @brief Called when the ship stops overlapping another actor
	 * @param otherActor The actor that is being ended from overlap
	 */
	virtual void NotifyActorEndOverlap(AActor* otherActor) override;

	/**
	 * @brief Plans a path to the next goal
	 * @param target The target actor to path plan to
	 * @return A success flag
	 */
	bool PlanPath (AActor* target);

	
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
	 * @brief Returns the current state of the agent
	 * @return The current state of the agent
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EAgentState GetAgentState () const;

	/**
	 * @brief Returns whether or not the morale has been reached
	 * @return A flag for whether or not the morale is reached
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMoraleReached () const;

	/**
	 * @brief Deposits fruit to the home base
	 * @param num The number of fruit to deposit
	 */
	void DepositFruit (int num = 1);

	/**
	 * @brief Deposits stone to the home base
	 * @param num The number of stone to deposit
	 */
	void DepositStone (int num = 1);

	/**
	 * @brief Deposits wood to the home base
	 * @param num The number of wood to deposit
	 */
	void DepositWood (int num = 1);

	/**
	 * @brief Deposits rum to the home base
	 * @param num The number of rum to deposit
	 */
	void DepositRum (int num = 1);

	/**
	 * @brief Collects the gold actor and removes it from the list
	 * @param gold The gold to collect
	 */
	void CollectGold (AGold* gold);

	/**
	 * @brief Tracks the current ship and displays in the viewport
	 */
	UFUNCTION(BlueprintCallable)
	void Track ();

	/**
	 * @brief Whether or not the ship is a merchant
	 * @return A merchant flag
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMerchant () const;
};