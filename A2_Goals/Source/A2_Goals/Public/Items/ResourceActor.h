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

	// The speed of movement
	UPROPERTY(EditAnywhere, Category = "Stats")
	int MaxResources;
	
	// The number of resources that exist
	UPROPERTY(BlueprintReadOnly)
	int ResourceCount;

	// The amount of resources increased per second
	UPROPERTY(EditAnywhere, Category = "Stats")
	float ResourceIncreaseTime;

	// The type of resources
	EGridType ResourceType;

	// The current grid X position
	int XPos;

	// The current grid Y position
	int YPos;


	/************************************************************/
    private:

	// The current time
	float CurrentTime = 0.0;
	
	
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

	/**
	 * @brief Returns whether or not the resource still has some resources
	 * @return A flag for whether resources exist
	 */
	UFUNCTION(BlueprintCallable)
	bool HasResources () const;

	/**
	 * @brief Takes a resource from this resource location 
	 * @param num The number of resources to take
	 */
	void TakeResource (int num = 1);

	/**
	 * @brief Returns the fraction of remaining resources
	 * @return The fraction of resources remaining
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ResourceFraction () const;

};
