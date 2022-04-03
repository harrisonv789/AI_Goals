/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "GOAPNode.h"

// Forward declarations of ships and any dependencies
class AShip;
class GOAPAction;

/**
 * @brief Defines a Goal Oriented Action Planner class, able to plan different actions
 * of some agent.
 */
class GOAPPlanner
{

	/************************************************************/
	protected:
	
	/**
	 * @brief This function builds out the node graph until exhausted all possible paths
	 * @param _allNodes A list of all action nodes
	 * @param _parent The parent node of this node
	 * @param _goalNodes A list of all target goal nodes to reach
	 * @param _availableActions A list of all actions that can be performed
	 * @param _goalState The required goal state and key for success
	 * @return A successful build flag
	 */
	static bool BuildGraphRecursive(TArray<GOAPNode*>& _allNodes, GOAPNode* _parent,
		TArray<GOAPNode*>& _goalNodes, const TSet<GOAPAction*>& _availableActions,
		TMap<FString, bool>& _goalState);

	/**
	 * @brief Function used for creating a subset of actions with a reward
	 * @param _availableActions A list of all available actions
	 * @param _removeAction The current action to remove
	 * @return A list of new actions as a subset
	 */
	static TSet<GOAPAction*> CreateActionSubset(const TSet<GOAPAction*>& _availableActions,
		GOAPAction* _removeAction);

	/**
	 * @brief Function compares a set of conditions to a state
	 * @param _conditions A list of all conditions to check
	 * @param _state The current state(s) to check
	 * @return Whether the condition exists within a state
	 */
	static bool CheckConditionsInState (TMap<FString, bool>& _conditions,
		TMap<FString, bool>& _state);

	/**
	 * @brief Combines a current state with a change (or effects) of an action
	 * @param _currentState A list of the current state keys and values
	 * @param _changes A list of all state changes that have occurred
	 * @return A new state list
	 */
	static TMap<FString, bool> PopulateNewState(const TMap<FString, bool>& _currentState,
		TMap<FString, bool>& _changes);

	
	/************************************************************/
	public:
	
	/**
	 * @brief Generates the action plan
	 * @param _ship A reference to the current agent
	 * @param _availableActions A list of available actions
	 * @param _plannedActions A list of currently planned actions
	 * @param _worldState The current state of the world or agent
	 * @param _goalState The target goal and state required
	 * @return A creation success flag
	 */
	static bool Plan(AShip* _ship, const TSet<GOAPAction*>& _availableActions,
		TQueue<GOAPAction*>& _plannedActions, TMap<FString, bool> _worldState,
		TMap<FString, bool> _goalState);
	
};
