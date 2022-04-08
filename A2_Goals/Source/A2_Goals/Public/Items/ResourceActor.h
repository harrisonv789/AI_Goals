/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#pragma once

#include "CoreMinimal.h"
#include "World/GridNode.h"
#include "GameFramework/Actor.h"
#include "ResourceActor.generated.h"

UCLASS()
class A2_GOALS_API AResourceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceActor();

	UPROPERTY(EditAnywhere)
	int ResourceCount;
	
	EGridType ResourceType;
	int XPos;
	int YPos;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
