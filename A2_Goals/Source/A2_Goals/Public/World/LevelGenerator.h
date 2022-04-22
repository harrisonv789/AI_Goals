/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */
 
#pragma once

#include "CoreMinimal.h"
#include "Items/Gold.h"
#include "GameFramework/Actor.h"
#include "GridNode.h"
#include "LevelGenerator.generated.h"

// Resolves a circular dependency
class AShip;

/**
 * @brief Base Level class that handles the spawning of all actors and the world
 */
UCLASS()
class A2_GOALS_API ALevelGenerator : public AActor
{
	GENERATED_BODY()

	// Maximum Size for World Map
	static constexpr int MAX_MAP_SIZE = 255;

	
	/************************************************************/
	public:

	// Grid Size in World Units
	static constexpr int GRID_SIZE_WORLD = 100;
	static constexpr int NUM_GOLD = 10;
	static constexpr int NUM_AGENTS = 6;
	
	// Sets default values for this actor's properties
	ALevelGenerator();

	// Size of the current Map in X coordinates
	UPROPERTY(BlueprintReadOnly)
	int MapSizeX;

	// Size of the current Map in Y coordinates
	UPROPERTY(BlueprintReadOnly)
	int MapSizeY;
	
	// This is a 2D Array for holding nodes for each part of the world
	GridNode* WorldArray[MAX_MAP_SIZE][MAX_MAP_SIZE];
	
	// A list of gold locations currently in the world
	UPROPERTY()
	TArray<FVector2D> GoldArray;

	// A list of gold actors current in the world
	UPROPERTY()
	TArray<AGold*> GoldActors;

	// A list of ships
	UPROPERTY(BlueprintReadOnly)
	TSet<AShip*> ShipFleet;

	// Actors for spawning into the world
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> PathDisplayBlueprint;
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> DeepBlueprint;
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> LandBlueprint;
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> ShallowBlueprint;
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> HomeBlueprint;
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> WoodBlueprint;
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> FruitBlueprint;
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> StoneBlueprint;
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> MerchantBlueprint;
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> GoldBlueprint;
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> ShipBlueprint;
	UPROPERTY(EditAnywhere, Category = "Entities")
	TSubclassOf<AActor> ResourceBlueprint;

	// The static camera actor
	UPROPERTY(EditAnywhere, Category = "Camera")
	AActor* Camera;

	// The offset to move the camera
	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector CameraPositionOffset = FVector(0, 0, 3600);

	// The total number of fruit collected
	UPROPERTY(BlueprintReadOnly)
	int TotalFruitCollected;

	// The total number of stone collected
	UPROPERTY(BlueprintReadOnly)
	int TotalStoneCollected;

	// The total number of wood collected
	UPROPERTY(BlueprintReadOnly)
	int TotalWoodCollected;

	// The current time of the simulation
	UPROPERTY(BlueprintReadOnly)
	float CurrentTime;

	
	/************************************************************/
	protected:
	
	/**
	 * @brief Called when the game first starts running
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Spawns all the actors associated with the grid
	 * @param grid A character grid
	 */
	void SpawnWorldActors(char grid[MAX_MAP_SIZE][MAX_MAP_SIZE]);

	/**
	 * @brief Generates all of the nodes in some grid
	 * @param grid A character grid
	 */
	void GenerateNodeGrid(char grid[MAX_MAP_SIZE][MAX_MAP_SIZE]);

	/**
	 * @brief Resets all of the nodes to empty, blank nodes in the grid
	 */
	void ResetAllNodes() const;

	/**
	 * @brief Calculates the nodal distance between two nodes
	 * @param first The starting node
	 * @param second The end node
	 * @return The distance in units
	 */
	float CalculateDistanceBetween(GridNode* first, GridNode* second) const;

	/**
	 * @brief Spawns the next gold on the grid
	 */
	void SpawnNextGold();


	/************************************************************/
	public:
	
	/**
	 * @brief Called every frame time
	 * @param deltaTime The time between each frame
	 */
	virtual void Tick(float deltaTime) override;

	/**
	 * @brief Generates a new world from a file
	 * @param worldArray An array of strings for the world
	 */
	UFUNCTION(BlueprintCallable)
	void GenerateWorldFromFile(TArray<FString> worldArray);

	/**
	 * @brief Checks to see if a selected gold is valid
	 * @param collectedGold 
	 * @return A valid flag
	 */
	bool IsGoldValid(AGold* collectedGold) const;

	/**
	 * @brief Collects some gold and removes it from the grid
	 * @param CollectedGold 
	 */
	void CollectGold(AGold* CollectedGold);

	/**
	 * @brief Calculates the path for a ship to travel to some goal
	 * @param targetShip The ship agent
	 * @param goalNode The goal to reach
	 */
	void CalculatePath(AShip* targetShip, GridNode* goalNode = nullptr);

	/**
	 * @brief Calculates a path for a ship to travel to some resource
	 * @param targetShip The ship agent
	 * @param resourceType The resource (nearest) to reach
	 */
	void CalculatePath(AShip* targetShip, EGridType resourceType);

	/**
	 * @brief Finds the nearest goal of some resource type
	 * @param xPos The X position
	 * @param yPos The Y position
	 * @param resourceType The resource looking for
	 * @param ignoreAgents Whether to ignore agents that are on locations
	 * @return The nearest grid node of some resource
	 */
	GridNode* CalculateNearestGoal(int xPos, int yPos, EGridType resourceType, bool ignoreAgents = false);

	/**
	 * @brief Finds a grid node with some actor reference
	 * @param actorResource The actor reference
	 * @return The grid node that contains the actor
	 */
	GridNode* FindGridNode(AActor* actorResource) const;

	/**
	 * @brief Removes all actors on some path
	 * @param currentShip The ship agent
	 * @param startNode The start node
	 * @param goalNode The goal node
	 */
	void ResetPath(AShip* currentShip, GridNode* startNode, GridNode* goalNode) const;

	/**
	 * @brief Renders the path to some goal
	 * @param currentShip The ship agent
	 * @param goalNode The target goal node
	 */
	void RenderPath(AShip* currentShip, const GridNode* goalNode) const;

	/**
	 * @brief Updates the agents location on the world grid and sets the references
	 * @param agent The agent reference
	 * @param prevX The previous X position
	 * @param prevY The previous Y position
	 * @param newX The new X position
	 * @param newY The new Y position
	 */
	void UpdateAgentLocation (AShip* agent, int prevX, int prevY, int newX, int newY) const;

	/**
	 * @brief Tracks a new agent and displays the tracking in the viewport
	 * @param agent The new agent to track
	 */
	UFUNCTION(BlueprintCallable)
	void TrackAgent (AShip* agent);
};
