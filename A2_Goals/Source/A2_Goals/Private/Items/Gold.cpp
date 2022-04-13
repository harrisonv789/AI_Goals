/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Items/Gold.h"

// Sets default values
AGold::AGold()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGold::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGold::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

