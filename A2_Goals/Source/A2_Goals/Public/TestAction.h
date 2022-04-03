/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "Planning/GOAPAction.h"

/**
 * @brief A test action sequence that will be replaced later
 * TODO: Remove this class
 */
class TestAction : public GOAPAction
{
	/************************************************************/
	protected:
	
	/**
	 * @brief Resets the action status
	 */
	virtual void Reset() override;

	/************************************************************/
	public:
	
	/**
	 * @brief Default constructor
	 */
	TestAction();

	/**
	 * @brief Default destructor
	 */
	virtual ~TestAction() override;

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
