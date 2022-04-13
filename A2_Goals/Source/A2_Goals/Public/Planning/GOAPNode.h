/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "Actions/GOAPAction.h"

/**
 * @brief Defines a series of data points that specify an action as a node
 */
struct GOAPNode
{
	/************************************************************/
	
	// The parent node
	GOAPNode* Parent;

	// The cost so far to reach this point
	float RunningCost;

	// The current state of the world, with all previous effects applied
	TMap<FString, bool> State;

	// A node is associated with an action. This is the reference to the action
	GOAPAction* Action;
		
};
