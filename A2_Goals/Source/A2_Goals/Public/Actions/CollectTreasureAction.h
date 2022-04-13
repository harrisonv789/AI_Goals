/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "Actions/GOAPAction.h"

/**
 * @brief An action that attempts to collect treasure
 */
class CollectTreasureAction : public GOAPAction
{
	/************************************************************/

	// How much treasure needs to be gathered?
	const int TreasureToGather = 1;

	// How long does it take to gather treasure?
	const float TimeToCollect = 1.0;

	// How many treasure have been gathered so far?
	int TreasureGathered;

	// How much time has elapsed inside of this action?
	float ActionTime;

	
	/************************************************************/
	
	/**
	 * @brief Resets the action status
	 */
	virtual void Reset() override;
	

	/************************************************************/
	public:
	
	/**
	 * @brief Default constructor
	 */
	CollectTreasureAction();

	/**
	 * @brief Default destructor
	 */
	virtual ~CollectTreasureAction() override;

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
