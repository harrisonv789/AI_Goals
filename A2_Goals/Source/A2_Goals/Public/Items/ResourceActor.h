/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "World/GridNode.h"
#include "GameFramework/Actor.h"
#include "ResourceActor.generated.h"

/**
 * @brief Actor class to represent a resource
 */
UCLASS()
class A2_GOALS_API AResourceActor : public AActor
{
	GENERATED_BODY()
	
	/************************************************************/
	public:
	
	// The number of resources that exist
	UPROPERTY(EditAnywhere)
	int ResourceCount;

	// The type of resources
	EGridType ResourceType;

	// The current grid X position
	int XPos;

	// The current grid Y position
	int YPos;
	
	
	/************************************************************/
	protected:
	
	/**
	 * @brief Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;


	/************************************************************/
	public:
	
	/**
	 * @brief Default constructor
	 */
	AResourceActor();

	/**
	 * @brief Called every frame
	 * @param deltaTime The time-step between ticks [s]
	 */
	virtual void Tick(float deltaTime) override;

};
