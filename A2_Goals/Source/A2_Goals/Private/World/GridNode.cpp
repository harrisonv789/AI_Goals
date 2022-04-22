/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "World/GridNode.h"

GridNode::GridNode()
{
	X = 0;
	Y = 0;

	GridType = DEEP_WATER;
	Parent = nullptr;
	ObjectAtLocation = nullptr;
	ResourceAtLocation = nullptr;
	AgentAtLocation = nullptr;

	ResourceNum = 0;

	G = 0;
	H = 0;
	F = 0;
	
	IsChecked = false;
}


float GridNode::GetTravelCost() const
{
	switch(GridType)
	{
		case DEEP_WATER:
			return 1;
		case SHALLOW_WATER:
			return 2;
		case LAND:
			return 999999;
		default:
			return 1;
	}
}
