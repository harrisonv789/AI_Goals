#pragma once

#include "GOAPNode.h"

// Forward declarations of ships and any dependencies
class AShip;
class GOAPAction;

class GOAPPlanner
{
public:
	// This function generates the action plan
	static bool Plan(AShip* _ship, const TSet<GOAPAction*>& _availableActions,
		TQueue<GOAPAction*>& _plannedActions, TMap<FString, bool> _worldState,
		TMap<FString, bool> _goalState);

protected:
	// This function builds out the node graph until exhausted all possible paths
	static bool BuildGraphRecursive(TArray<GOAPNode*>& _allNodes, GOAPNode* _parent,
		TArray<GOAPNode*>& _goalNodes, const TSet<GOAPAction*>& _availableActions,
		TMap<FString, bool>& _goalState);

	// Function used for creating a subset of actions with a reward
	static TSet<GOAPAction*> CreateActionSubset(const TSet<GOAPAction*>& _availableActions,
		GOAPAction* _removeAction);

	// Function compares a set of conditions to a state
	static bool CheckConditionsInState (TMap<FString, bool>& _conditions,
		TMap<FString, bool>& _state);

	// Combines a current state with a change (or effects) of an action
	static TMap<FString, bool> PopulateNewState(const TMap<FString, bool>& _currentState,
		TMap<FString, bool>& _changes);
};
