/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "Actions/GOAPAction.h"
#include "World/GridType.h"

/**
 * @brief An action that attempts to deposit some resource at the home base
 */
class DepositResourceAction : public GOAPAction
{
	/************************************************************/

	// The resource looking for
	EGridType ResourceType;
	
	// How much resources will be deposited
	int ResourcesToDeposit = 50;

	// How long does it take to deposit treasure?
	const float TimeToDeposit = 0.5f;

	// How much resource has been deposited so far?
	int ResourcesDeposited;

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
	 * @brief Deposits some resource of some type
	 * @param resourceType The type of resource to look for
	 */
	DepositResourceAction(EGridType resourceType, int resourcesToDeposit);

	/**
	 * @brief Default destructor
	 */
	virtual ~DepositResourceAction() override;

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

	/**
	 * @brief Updates the resource type to look for
	 * @param resourceType A new resource grid type
	 */
	void SetResourceType(EGridType resourceType);
	
};
