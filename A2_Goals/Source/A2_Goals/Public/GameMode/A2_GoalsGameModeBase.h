/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "A2_GoalsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class A2_GOALS_API AA2_GoalsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Utility Functions")
		TArray<FString> GetMapFileList();

	UFUNCTION(BlueprintCallable, Category = "Utility Functions")
		FString GetRandomMapText();

	UFUNCTION(BlueprintCallable, Category = "Utility Functions")
		TArray<FString> GetMapArray();
};
