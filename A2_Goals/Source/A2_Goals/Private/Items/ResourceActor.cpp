/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Items/ResourceActor.h"

// Sets default values
AResourceActor::AResourceActor()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	ResourceCount = 500;
}

// Called when the game starts or when spawned
void AResourceActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AResourceActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

