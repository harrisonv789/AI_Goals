/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "GOAPNode.h"
#include "Actions/GOAPAction.h"

// Forward declarations of ships and any dependencies
class AShip;
class GOAPAction;

/**
 * @brief Defines a Goal Oriented Action Planner class, able to plan different actions
 * of some agent. This is a static class for static functions.
 */
class GOAPPlanner
{

	/************************************************************/
	protected:
	
	/**
	 * @brief This function builds out the node graph until exhausted all possible paths
	 * @param allNodes A list of all action nodes
	 * @param parent The parent node of this node
	 * @param goalNodes A list of all target goal nodes to reach
	 * @param availableActions A list of all actions that can be performed
	 * @param goalState The required goal state and key for success
	 * @return A successful build flag
	 */
	static bool BuildGraphRecursive(TArray<GOAPNode*>& allNodes, GOAPNode* parent,
		TArray<GOAPNode*>& goalNodes, const TSet<GOAPAction*>& availableActions,
		TMap<FString, bool>& goalState);

	/**
	 * @brief Function used for creating a subset of actions with a reward
	 * @param availableActions A list of all available actions
	 * @param removeAction The current action to remove
	 * @return A list of new actions as a subset
	 */
	static TSet<GOAPAction*> CreateActionSubset(const TSet<GOAPAction*>& availableActions,
		GOAPAction* removeAction);

	/**
	 * @brief Function compares a set of conditions to a state
	 * @param conditions A list of all conditions to check
	 * @param state The current state(s) to check
	 * @return Whether the condition exists within a state
	 */
	static bool CheckConditionsInState (TMap<FString, bool>& conditions,
		TMap<FString, bool>& state);

	/**
	 * @brief Combines a current state with a change (or effects) of an action
	 * @param currentState A list of the current state keys and values
	 * @param changes A list of all state changes that have occurred
	 * @return A new state list
	 */
	static TMap<FString, bool> PopulateNewState(const TMap<FString, bool>& currentState,
		TMap<FString, bool>& changes);

	
	/************************************************************/
	public:
	
	/**
	 * @brief Generates the action plan
	 * @param ship A reference to the current agent
	 * @param availableActions A list of available actions
	 * @param plannedActions A list of currently planned actions
	 * @param worldState The current state of the world or agent
	 * @param goalState The target goal and state required
	 * @return A creation success flag
	 */
	static bool Plan(AShip* ship, const TSet<GOAPAction*>& availableActions,
		TQueue<GOAPAction*>& plannedActions, TMap<FString, bool> worldState,
		TMap<FString, bool> goalState);
	
};
