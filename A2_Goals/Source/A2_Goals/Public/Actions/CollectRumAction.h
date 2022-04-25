/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "Actions/GOAPAction.h"
#include "World/LevelGenerator.h"

/**
 * @brief An action that collects some rum from a home base when low
 */
class CollectRumAction : public GOAPAction
{
	/************************************************************/
	public:
	
	// How much rum is collected when low on rum
	const int RumToCollect = 10;

	// How long does it take to gather rum?
	const float TimeToCollect = 0.2f;

	// How much rum has been gathered so far?
	int RumGathered;

	// How much time has elapsed inside of this action?
	float ActionTime;

	// The current level
	ALevelGenerator* Level;

	
	/************************************************************/
	
	/**
	 * @brief Resets the action status
	 */
	virtual void Reset() override;
	

	/************************************************************/
	public:
	
	/**
	 * @brief Collects some rum
	 */
	CollectRumAction();

	/**
	 * @brief Default destructor
	 */
	virtual ~CollectRumAction() override;

	/**
	 * @brief Returns whether the action is completed or not
	 * @return A success flag
	 */
	virtual bool IsActionDone() override;

	/**
	 * @brief Checks if the current preconditions are valid
	 * @param ship A reference to the ship agent
	 * @return A state flag for the conditions
	 */
	virtual bool CheckProceduralPreconditions(AShip* ship) override;

	/**
	 * @brief Performs the action on a particular agent
	 * @param ship A reference to the ship agent
	 * @param deltaTime The current time difference for the action to run at
	 * @return A success flag for the action
	 */
	virtual bool PerformAction(AShip* ship, float deltaTime) override;

	/**
	 * @brief Whether or not the action requires something in range
	 * @return In Range flag
	 */
	virtual bool RequiresInRange() override;
	
};
