#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"

struct GOAPNode
{
	public:
	
	// The parent node
	GOAPNode* Parent;

	// The cost so far to reach this point
	float RunningCost;

	// The current state of the world, with all previous effects applied
	TMap<FString, bool> State;

	// A node is associated with an action. This is the reference to the action
	GOAPAction* Action;
		
};
