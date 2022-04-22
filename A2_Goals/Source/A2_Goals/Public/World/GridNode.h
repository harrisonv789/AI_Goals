/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "GridType.h"
#include "Items/ResourceActor.h"

/**
 * Stores information about a specific GridNode in the world
 */
class A2_GOALS_API GridNode
{
	
	/************************************************************/
	public:

	// Position in Grid
	int X;
	int Y;

	// Informed Search Variables
	int G;
	float H;
	float F;

	// Number of resources associated with node
	int ResourceNum;

	// Type of grid space
	EGridType GridType;
	
	// Pointer to previous Node (Only used in searching)
	GridNode* Parent;

	// Object at current location
	AActor* ObjectAtLocation;
	AActor* ResourceAtLocation;
	AActor* AgentAtLocation;

	// Whether or not the grid square has been checked by the search
	bool IsChecked;

	/**
	 * @brief The default constructor of the GridNode
	 */
	GridNode();

	/**
	 * @brief Returns the cost to travel to this grid
	 * @return The travel cost
	 */
	float GetTravelCost() const;
};
