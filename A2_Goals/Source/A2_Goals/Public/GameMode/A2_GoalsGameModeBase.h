/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "A2_GoalsGameModeBase.generated.h"

/**
 * The main GameMode used in the Game
 */
UCLASS()
class A2_GOALS_API AA2_GoalsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	/************************************************************/
	public:
	
	/**
	 * @brief Creates a list of map files to load
	 * @return The list of map file destinations
	 */
	UFUNCTION(BlueprintCallable, Category = "Utility Functions")
	TArray<FString> GetMapFileList();

	/**
	 * @brief Gets a random map to chose from
	 * @return A random map string
	 */
	UFUNCTION(BlueprintCallable, Category = "Utility Functions")
	FString GetRandomMapText();

	/**
	 * @brief Gets the characters from the map array
	 * @return An array of strings from the map
	 */
	UFUNCTION(BlueprintCallable, Category = "Utility Functions")
	TArray<FString> GetMapArray();
};
