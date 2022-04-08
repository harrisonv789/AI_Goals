/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Planning/GOAPPlanner.h"
#include "Actions/GOAPAction.h"

bool GOAPPlanner::Plan(AShip* _ship, const TSet<GOAPAction*>& _availableActions, TQueue<GOAPAction*>& _plannedActions,
	TMap<FString, bool> _worldState, TMap<FString, bool> _goalState)
{
	// Create an array to hold all nodes generated
	TArray<GOAPNode*> allNodes;

	// Reset all action states
	for (const auto action : _availableActions)
	{
		action->DoReset();
	}

	// Clear the planned acion queue
	_plannedActions.Empty();

	// Get usable actions
	TSet<GOAPAction*> usableActions;
	for (const auto action : _availableActions)
	{
		if (action->CheckProceduralPreconditions(_ship))
		{
			usableActions.Add(action);
		}
	}

	// Build a tree of actions
	TArray<GOAPNode*> goalNodes;

	// Define a starting node and reset it
	GOAPNode* start = new GOAPNode;
	start->Parent = nullptr;
	start->RunningCost = 0;
	start->State = _worldState;
	start->Action = nullptr;

	// Call the build graph function, returning true if a path is found
	const bool isSuccessful = BuildGraphRecursive(allNodes, start, goalNodes, usableActions, _goalState);

	// If failed
	if (!isSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("GOAPPLANNER: No Plan Found"));
		return false;
	}

	// Get the cheapest goal
	GOAPNode* cheapestNode = nullptr;
	for (const auto node : goalNodes)
	{
		if (cheapestNode)
		{
			if (node->RunningCost < cheapestNode->RunningCost)
			{
				cheapestNode = node;
			}
		} else
		{
			cheapestNode = node;
		}
	}

	// Work back from the cheapest goal to start
	TArray<GOAPAction*> results;
	GOAPNode* currentNode = cheapestNode;

	while (currentNode)
	{
		if (currentNode->Action)
		{
			results.Insert(currentNode->Action, 0);
		}
		currentNode = currentNode->Parent;
	}

	// Build queue from result
	for (auto action : results)
	{
		_plannedActions.Enqueue(action);
	}

	// Clean up nodes
	allNodes.Empty();

	// Return a success
	return true;
}


bool GOAPPlanner::BuildGraphRecursive(TArray<GOAPNode*>& _allNodes, GOAPNode* _parent, TArray<GOAPNode*>& _goalNodes,
	const TSet<GOAPAction*>& _availableActions, TMap<FString, bool>& _goalState)
{
	// For every action that is available
	for (auto action : _availableActions)
	{
		// Check to see if the preconditions of a state allow it to run
		if (CheckConditionsInState(action->Preconditions, _parent->State))
		{
			// Create a new updated world state based on the current state and action effects
			TMap<FString, bool> currentState = PopulateNewState(_parent->State, action->Effects);

			// Create a new GOAP node object for this action
			GOAPNode* node = new GOAPNode;
			node->Parent = _parent;
			node->RunningCost = _parent->RunningCost + action->ActionCost;
			node->State = currentState;
			node->Action = action;

			// Add it to our main list of nodes for deletion later
			_allNodes.Add(node);

			// Check conditions to see if we have a match for our goal
			if (CheckConditionsInState(_goalState, currentState))
			{
				// Found goal
				_goalNodes.Add(node);

				// Return true and break loop
				return true;
			}

			// If no match
			else
			{
				// Create a new subset of available actions without the current state
				TSet<GOAPAction*> actionSubset = CreateActionSubset(_availableActions, action);

				// Call this function recursively
				if (BuildGraphRecursive(_allNodes, node, _goalNodes, actionSubset, _goalState))
				{
					return true;
				}
			}
		}
	}

	// If we find nothing, return false
	return false;
}


TSet<GOAPAction*> GOAPPlanner::CreateActionSubset(const TSet<GOAPAction*>& _availableActions, GOAPAction* _removeAction)
{
	TSet<GOAPAction*> newActionSet;

	for (auto action : _availableActions)
	{
		if (action != _removeAction)
		{
			newActionSet.Add(action);
		}
	}

	return newActionSet;
}


bool GOAPPlanner::CheckConditionsInState(TMap<FString, bool>& _conditions, TMap<FString, bool>& _state)
{
	for (auto condition : _conditions)
	{
		bool* currentStateCondition = _state.Find(condition.Key);

		if (currentStateCondition)
		{
			if (condition.Value != *currentStateCondition)
			{
				return false;
			}
		} else
		{
			return false;
		}
	}

	return true;
}


TMap<FString, bool> GOAPPlanner::PopulateNewState(const TMap<FString, bool>& _currentState,
	TMap<FString, bool>& _changes)
{
	TMap<FString, bool> newState = _currentState;

	for (auto pairs : _changes)
	{
		newState.Add(pairs.Key, pairs.Value);
	}

	return newState;
}
