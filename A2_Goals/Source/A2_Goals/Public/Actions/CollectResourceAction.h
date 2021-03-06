/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "Actions/GOAPAction.h"
#include "World/GridType.h"

/**
 * @brief An action that attempts to collect some resource
 */
class CollectResourceAction : public GOAPAction
{
	/************************************************************/
	public:

	// The resource looking for
	EGridType ResourceType;
	
	// How much resource needs to be gathered?
	const int ResourceToGather = 50;

	// How long does it take to gather treasure?
	const float TimeToCollect = 0.2f;

	// How much resource has been gathered so far?
	int ResourceGathered;

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
	 * @brief Collects a resource of some type
	 * @param resourceType The type of resource to look for
	 */
	CollectResourceAction(EGridType resourceType);

	/**
	 * @brief Default destructor
	 */
	virtual ~CollectResourceAction() override;

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
