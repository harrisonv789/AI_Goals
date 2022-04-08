/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

/**
 * @brief Describes different types of environmental tiles
 */
UENUM(Blueprintable, BlueprintType)
enum EGridType
{
	DEEP_WATER			= 0,
	LAND				= 1,
	SHALLOW_WATER		= 2,
	HOME				= 3,
	WOOD_RESOURCE		= 4,
	STONE_RESOURCE		= 5,
	FRUIT_RESOURCE		= 6,
	MERCHANT_RESOURCE	= 7,
	GOLD_RESOURCE		= 8
};