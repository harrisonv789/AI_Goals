/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/LevelGenerator.h"
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

public:	
	// Sets default values for this actor's properties
	AShip();
	bool GeneratePath = true;
	bool FinishedMoving = false;
	TArray<GridNode*> Path;
	ALevelGenerator* Level;
	int morale = 100;

	int xPos;
	int yPos;

	EGridType ResourceType;

	UPROPERTY()
	TArray<AActor*> PathDisplayActors;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
