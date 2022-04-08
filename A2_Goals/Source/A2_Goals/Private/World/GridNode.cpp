/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "World/GridNode.h"

GridNode::GridNode()
{
	X = 0;
	Y = 0;

	GridType = DeepWater;
	Parent = nullptr;
	ObjectAtLocation = nullptr;

	G = 0;
	H = 0;
	F = 0;
	
	IsChecked = false;
}

float GridNode::GetTravelCost() const
{
	switch(GridType)
	{
		case DeepWater:
			return 1;
		case ShallowWater:
			return 2;
		case Land:
			return 999999;
		default:
			return 1;
	}
}
