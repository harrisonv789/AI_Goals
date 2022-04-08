/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gold.generated.h"

UCLASS()
class A2_GOALS_API AGold : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGold();

	bool IsRetrieved;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
