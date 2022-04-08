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
	DeepWater			= 0,
	Land				= 1,
	ShallowWater		= 2,
	Home				= 3,
	WoodResource		= 4,
	StoneResource		= 5,
	FruitResource		= 6,
	MerchantResource	= 7,
	GoldResource		= 8
};