/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gold.generated.h"

/**
 * @brief Class for storing a gold item
 */
UCLASS()
class A2_GOALS_API AGold : public AActor
{
	GENERATED_BODY()

	/************************************************************/
	public:

	// A flag for whether the gold has been retrieved or not
	bool IsRetrieved;
	

	/************************************************************/
	protected:
	
	/**
	 * @brief Called when the game first starts running
	 */
	virtual void BeginPlay() override;


	/************************************************************/
	public:
	
	/**
	 * @brief Default constructor
	 */
	AGold();

	/**
	 * @brief Called every time the game ticks
	 * @param deltaTime The time-step [s]
	 */
	virtual void Tick(float deltaTime) override;

};
