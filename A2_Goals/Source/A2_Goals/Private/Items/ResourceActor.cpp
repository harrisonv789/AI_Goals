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

	// Set the default replenish time
	ResourceIncreaseTime = 0.0;

	// Store the resources
	MaxResources = 500;
}

// Called when the game starts or when spawned
void AResourceActor::BeginPlay()
{
	Super::BeginPlay();

	ResourceCount = MaxResources;
}

// Called every frame
void AResourceActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (CurrentTime > ResourceIncreaseTime && ResourceIncreaseTime > 0.0)
	{
		CurrentTime = 0.0;
		ResourceCount += 1;

		// Clamp the resources at the max
		if (ResourceCount > MaxResources)
			ResourceCount = MaxResources;
	}
	else
	{
		// Increase the current time
		CurrentTime += deltaTime;
	}
}

bool AResourceActor::HasResources() const
{
	return ResourceCount > 0;
}

void AResourceActor::TakeResource(int num)
{
	ResourceCount -= num;
}

float AResourceActor::ResourceFraction() const
{
	return static_cast<float>(ResourceCount) / static_cast<float>(MaxResources);
}

