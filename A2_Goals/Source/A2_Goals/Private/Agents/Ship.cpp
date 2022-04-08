/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "Agents/Ship.h"
#include "Items/Gold.h"
#include "World/LevelGenerator.h"

// Sets default values
AShip::AShip()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 100;
	Tolerance = 1;
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (morale == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, FString::Printf(TEXT("Your ship has undergone a mutiny! Ship: %s"), *GetName()));
		UE_LOG(LogTemp, Warning, TEXT("Your ship has undergone a mutiny!"));
	}

	if(!FinishedMoving)
	{
		if (Path.Num() > 0)
		{
			FVector CurrentPosition = GetActorLocation();

			float TargetXPos = Path[0]->X * ALevelGenerator::GRID_SIZE_WORLD;
			float TargetYPos = Path[0]->Y * ALevelGenerator::GRID_SIZE_WORLD;

			FVector TargetPosition(TargetXPos, TargetYPos, CurrentPosition.Z);

			FVector Direction = TargetPosition - CurrentPosition;
			Direction.Normalize();

			CurrentPosition += Direction * MoveSpeed * DeltaTime;
			SetActorLocation(CurrentPosition);

			if (FVector::Dist(CurrentPosition, TargetPosition) <= Tolerance)
			{
				xPos = Path[0]->X;
				yPos = Path[0]->Y;
				CurrentPosition = TargetPosition;
				Path.RemoveAt(0);
				morale--;

				FinishedMoving = true;
			}
		}
		else
		{
			FinishedMoving = true;
			Level->CalculatePath(this);
		}
	}
}
