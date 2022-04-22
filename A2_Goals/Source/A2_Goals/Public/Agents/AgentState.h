/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

/**
 * @brief Describes different states the agent could be in
 */
UENUM(Blueprintable, BlueprintType)
enum EAgentState
{
	NOTHING				= 0,
	IDLE				= 1,
	MOVE				= 2,
	ACTION				= 3,
	COMPLETE			= 4,
};