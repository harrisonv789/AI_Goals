/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	
	/**
	 * @brief Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;
	

	/************************************************************/
	public:
	
	/**
	 * @brief Sets default values for this actor's properties
	 */
	AShip();

	/**
	 * @brief Called every frame
	 * @param DeltaTime [s] - The time step 
	 */
	virtual void Tick(float DeltaTime) override;
};
