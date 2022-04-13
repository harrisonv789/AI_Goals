/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Planning/GOAPPlanner.h"
#include "Actions/GOAPAction.h"

bool GOAPPlanner::Plan(AShip* ship, const TSet<GOAPAction*>& availableActions, TQueue<GOAPAction*>& plannedActions,
	TMap<FString, bool> worldState, TMap<FString, bool> goalState)
{
	// Create an array to hold all nodes generated
	TArray<GOAPNode*> allNodes;

	// Reset all action states
	for (const auto action : availableActions)
	{
		action->DoReset();
	}

	// Clear the planned action queue
	plannedActions.Empty();

	// Get usable actions
	TSet<GOAPAction*> usableActions;
	for (const auto action : availableActions)
	{
		if (action->CheckProceduralPreconditions(ship))
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
	start->State = worldState;
	start->Action = nullptr;

	// Call the build graph function, returning true if a path is found
	const bool isSuccessful = BuildGraphRecursive(allNodes, start, goalNodes, usableActions, goalState);

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
		plannedActions.Enqueue(action);
	}

	// Clean up nodes
	allNodes.Empty();

	// Return a success
	return true;
}


bool GOAPPlanner::BuildGraphRecursive(TArray<GOAPNode*>& allNodes, GOAPNode* parent, TArray<GOAPNode*>& goalNodes,
	const TSet<GOAPAction*>& availableActions, TMap<FString, bool>& goalState)
{
	// For every action that is available
	for (const auto action : availableActions)
	{
		// Check to see if the preconditions of a state allow it to run
		if (CheckConditionsInState(action->Preconditions, parent->State))
		{
			// Create a new updated world state based on the current state and action effects
			TMap<FString, bool> currentState = PopulateNewState(parent->State, action->Effects);

			// Create a new GOAP node object for this action
			GOAPNode* node = new GOAPNode;
			node->Parent = parent;
			node->RunningCost = parent->RunningCost + action->ActionCost;
			node->State = currentState;
			node->Action = action;

			// Add it to our main list of nodes for deletion later
			allNodes.Add(node);

			// Check conditions to see if we have a match for our goal
			if (CheckConditionsInState(goalState, currentState))
			{
				// Found goal
				goalNodes.Add(node);

				// Return true and break loop
				return true;
			}

			// If no match
			else
			{
				// Create a new subset of available actions without the current state
				TSet<GOAPAction*> actionSubset = CreateActionSubset(availableActions, action);

				// Call this function recursively
				if (BuildGraphRecursive(allNodes, node, goalNodes, actionSubset, goalState))
				{
					return true;
				}
			}
		}
	}

	// If we find nothing, return false
	return false;
}


TSet<GOAPAction*> GOAPPlanner::CreateActionSubset(const TSet<GOAPAction*>& availableActions, GOAPAction* removeAction)
{
	TSet<GOAPAction*> newActionSet;

	for (auto action : availableActions)
	{
		if (action != removeAction)
		{
			newActionSet.Add(action);
		}
	}

	return newActionSet;
}


bool GOAPPlanner::CheckConditionsInState(TMap<FString, bool>& conditions, TMap<FString, bool>& state)
{
	for (auto condition : conditions)
	{
		bool* currentStateCondition = state.Find(condition.Key);

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


TMap<FString, bool> GOAPPlanner::PopulateNewState(const TMap<FString, bool>& currentState,
	TMap<FString, bool>& changes)
{
	TMap<FString, bool> newState = currentState;

	for (auto pairs : changes)
	{
		newState.Add(pairs.Key, pairs.Value);
	}

	return newState;
}
