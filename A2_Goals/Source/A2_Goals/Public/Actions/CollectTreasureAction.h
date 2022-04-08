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
	private:

	// How much treasure needs to be gathered?
	const int TreasureToGather = 1;

	// How long does it take to gather treasure?
	const int TimeToCollect = 1;

	// How many treasure have been gathered so far?
	int TreasureGathered;

	// How much time has elapsed inside of this action?
	float ActionTime;

	
	/************************************************************/
	private:
	
	/**
	 * @brief Resets the action status
	 */
	void Reset() override;
	

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
	 * @brief 
	 * @return 
	 */
	virtual bool IsActionDone() override;

	// Pure virtual functions that the child classes must inherit
	virtual bool CheckProceduralPreconditions(AShip* _ship) override;

	// Performs the action
	virtual bool PerformAction(AShip* _ship, float _deltaTime) override;

	// Whether or not the action requires something in range
	virtual bool RequiresInRange() override;
};
