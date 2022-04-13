/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

// Forward declaration of the ship class
class AShip;

/**
 * @brief Defines a particular Goal Oriented Action Planner action
 *		that can be implemented by an agent.
 */
class GOAPAction
{

	/************************************************************/
	protected:

	// A status for whether the action is in range
	bool InRange;

	
	/************************************************************/
	public:
	
	// List of the conditions required for this action to be valid
	TMap<FString, bool> Preconditions;

	// The changes this action has on the world or agent
	TMap<FString, bool> Effects;

	// The cost of the action
	float ActionCost;

	// The target the action works on
	AActor* Target;


	/************************************************************/
	protected:
	
	/**
	 * @brief Resets the action for a particular subclass.
	 * DoReset in the below section handles calling this function
	 */
	virtual void Reset() = 0;


	/************************************************************/
	public:
	
	/**
	 * @brief Default constructor
	 */
	GOAPAction();

	/**
	 * @brief Default destructor
	 */
	virtual ~GOAPAction();

	/**
	 * @brief Resets the conditions of the action back to its initial state
	 */
	void DoReset();

	/**
	 * @brief Adds a precondition state to the current condition list
	 * @param name The key of the new condition
	 * @param state The starting state flag of the condition
	 */
	void AddPrecondition(FString name, bool state);

	/**
	 * @brief Removes a precondition state from the condition list
	 * @param name The key of the new condition
	 */
	void RemovePrecondition(FString name);

	/**
	 * @brief Adds a new effect of the action to the current effects list
	 * @param name The key of the new effect
	 * @param state The starting state flag of the effect
	 */
	void AddEffect(FString name, bool state);

	/**
	 * @brief Removes an effect from the effects list
	 * @param name The key of the new effect
	 */
	void RemoveEffect(FString name);

	/**
	 * @brief Checks whether or not the current action is within range
	 * @return If the action is within range
	 */
	bool IsInRange() const;

	/**
	 * @brief Sets the state of whether the action is within range
	 * @param range The new range flag
	 */
	void SetInRange (bool range);

	/**
	 * @brief Returns whether the action is completed or not
	 * @return A success flag
	 */
	virtual bool IsActionDone() = 0;

	/**
	 * @brief Checks if the current preconditions are valid
	 * @param ship A reference to the ship agent
	 * @return A state flag for the conditions
	 */
	virtual bool CheckProceduralPreconditions(AShip* ship) = 0;

	/**
	 * @brief Performs the action on a particular agent
	 * @param ship A reference to the ship agent
	 * @param deltaTime The current time difference for the action to run at
	 * @return A success flag for the action
	 */
	virtual bool PerformAction(AShip* ship, float deltaTime) = 0;

	/**
	 * @brief Whether or not the action requires something in range
	 * @return In Range flag
	 */
	virtual bool RequiresInRange() = 0;
	
};
