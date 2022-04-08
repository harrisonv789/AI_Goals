/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "GridType.h"
#include "GameFramework/Actor.h"

/**
 * 
 */
class A2_GOALS_API GridNode
{

public:

	GridNode();

	float GetTravelCost() const;

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

	// Whether or not the grid square has been checked by the search
	bool IsChecked;
};
