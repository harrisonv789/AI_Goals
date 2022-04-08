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

UCLASS()
class A2_GOALS_API ALevelGenerator : public AActor
{
	GENERATED_BODY()

	// Maximum Size for World Map
	static const int MAX_MAP_SIZE = 255;

public:

	// Grid Size in World Units
	static const int GRID_SIZE_WORLD = 100;
	static const int NUM_FOOD = 10;
	static const int NUM_AGENTS = 5;
	
	// Sets default values for this actor's properties
	ALevelGenerator();

	UPROPERTY(BlueprintReadOnly)
		int MapSizeX;
	UPROPERTY(BlueprintReadOnly)
		int MapSizeY;
	
	UPROPERTY()
		TArray<FVector2D> GoldArray;

	// This is a 2D Array for holding nodes for each part of the world
	GridNode* WorldArray[MAX_MAP_SIZE][MAX_MAP_SIZE];

	UPROPERTY()
		TArray<AGold*> GoldActors;

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
	UPROPERTY(EditAnywhere, Category = "Entities")
		AActor* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnWorldActors(char Grid[MAX_MAP_SIZE][MAX_MAP_SIZE]);

	void GenerateNodeGrid(char Grid[MAX_MAP_SIZE][MAX_MAP_SIZE]);
	void ResetAllNodes();

	float CalculateDistanceBetween(GridNode* first, GridNode* second);

	void SpawnNextGold();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void GenerateWorldFromFile(TArray<FString> WorldArray);
	
	//void CalculateBFS();
	//int SearchCount = 0;
	//bool IsPathCalculated;
	//GridNode* StartNode;
	//GridNode* GoalNode;

	//void ResetPath();
	//void RenderPath();
	//void DetailPath();
	//void ResetPath();
	bool IsGoldValid(AGold* CollectedGold);
	void CollectGold(AGold* CollectedGold);
	//AShip* Ship;

	// Assignment 2 Additions

	// Variables
	TSet<AShip*> ShipFleet;
	
	// Functions
	void CalculatePath(AShip* TargetShip, GridNode* GoalNode = nullptr);
	void CalculatePath(AShip* TargetShip, EGridType ResourceType);
	GridNode* CalculateNearestGoal(int XPos, int YPos, EGridType ResourceType);
	GridNode* FindGridNode(AActor* ActorResource);

	void ResetPath(AShip* CurrentShip, GridNode* StartNode, GridNode* GoalNode);
	void DetailPath(AShip* CurrentShip, GridNode* StartNode, GridNode* GoalNode);
	void RenderPath(AShip* CurrentShip, GridNode* GoalNode);
};
