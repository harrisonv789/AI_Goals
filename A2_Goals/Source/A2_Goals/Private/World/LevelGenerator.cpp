/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "World/LevelGenerator.h"
#include "Items/ResourceActor.h"
#include "Engine/World.h"
#include "Agents/Ship.h"
#include "Runtime/Engine/Public/EngineGlobals.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;
	ResourceBlueprint = AResourceActor::StaticClass();
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool ResetMovement = true;

	for(auto Ship : ShipFleet)
	{
		if(!Ship->FinishedMoving)
		{
			ResetMovement = false;
		}
	}

	if(ResetMovement)
	{
		for(auto Ship : ShipFleet)
		{
			Ship->FinishedMoving = false;
		}
	}

	if(GoldActors.Num() < NUM_FOOD)
	{
		SpawnNextGold();
	}
}

void ALevelGenerator::GenerateWorldFromFile(TArray<FString> WorldArrayStrings)
{
	// If empty array exit immediately something is horribly wrong
	if(WorldArrayStrings.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("World Array is empty!"));
		return;
	}

	// Second line is Height (aka Y value)
	FString Height = WorldArrayStrings[1];
	Height.RemoveFromStart("Height ");
	MapSizeY = FCString::Atoi(*Height);
	UE_LOG(LogTemp, Warning, TEXT("Height: %d"), MapSizeY);

	// Third line is Width (aka X value)
	FString Width = WorldArrayStrings[2];
	Width.RemoveFromStart("width ");
	MapSizeX = FCString::Atoi(*Width);
	UE_LOG(LogTemp, Warning, TEXT("Width: %d"), MapSizeX);

	char CharMapArray[MAX_MAP_SIZE][MAX_MAP_SIZE];
	
	// Read through the Map section for create the CharMapArray
	for (int LineNum = 4; LineNum < MapSizeY + 4; LineNum++)
	{
		for (int CharNum = 0; CharNum < WorldArrayStrings[LineNum].Len(); CharNum++)
		{
			CharMapArray[LineNum-4][CharNum] = WorldArrayStrings[LineNum][CharNum];
		}
	}

	GenerateNodeGrid(CharMapArray);
	SpawnWorldActors(CharMapArray);
}

void ALevelGenerator::SpawnWorldActors(char Grid[MAX_MAP_SIZE][MAX_MAP_SIZE])
{
	UWorld* World = GetWorld();

	// Make sure that all blueprints are connected. If not then fail
	if(DeepBlueprint && ShallowBlueprint && LandBlueprint && WoodBlueprint && StoneBlueprint && FruitBlueprint && MerchantBlueprint)
	{
		AResourceActor* TempResource = nullptr;
		// For each grid space spawn an actor of the correct type in the game world
		for(int x = 0; x < MapSizeX; x++)
		{
			for (int y = 0; y < MapSizeY; y++)
			{
				float XPos = x * GRID_SIZE_WORLD;
				float YPos = y * GRID_SIZE_WORLD;

				FVector Position(XPos, YPos, 0);

				switch (Grid[x][y])
				{
					case '.':
						World->SpawnActor(DeepBlueprint, &Position, &FRotator::ZeroRotator);
						break;
					case '@':
						World->SpawnActor(LandBlueprint, &Position, &FRotator::ZeroRotator);
						break;
					case 'H':
						World->SpawnActor(ShallowBlueprint, &Position, &FRotator::ZeroRotator);
						break;
					case 'W':
						World->SpawnActor(WoodBlueprint, &Position, &FRotator::ZeroRotator);
						TempResource = Cast<AResourceActor>(World->SpawnActor(ResourceBlueprint, &Position, &FRotator::ZeroRotator));
						TempResource->ResourceType = EGridType::WOOD_RESOURCE;
						TempResource->XPos = x;
						TempResource->YPos = y;
						WorldArray[x][y]->ResourceAtLocation = TempResource;
						break;
					case 'S':
						World->SpawnActor(StoneBlueprint, &Position, &FRotator::ZeroRotator);
						TempResource = Cast<AResourceActor>(World->SpawnActor(ResourceBlueprint, &Position, &FRotator::ZeroRotator));
						TempResource->ResourceType = EGridType::STONE_RESOURCE;
						TempResource->XPos = x;
						TempResource->YPos = y;
						WorldArray[x][y]->ResourceAtLocation = TempResource;
						break;
					case 'F':
						World->SpawnActor(FruitBlueprint, &Position, &FRotator::ZeroRotator);
						TempResource = Cast<AResourceActor>(World->SpawnActor(ResourceBlueprint, &Position, &FRotator::ZeroRotator));
						TempResource->ResourceType = EGridType::FRUIT_RESOURCE;
						TempResource->XPos = x;
						TempResource->YPos = y;
						WorldArray[x][y]->ResourceAtLocation = TempResource;
						break;
					case 'M':
						World->SpawnActor(MerchantBlueprint, &Position, &FRotator::ZeroRotator);
						TempResource = Cast<AResourceActor>(World->SpawnActor(ResourceBlueprint, &Position, &FRotator::ZeroRotator));
						TempResource->ResourceType = EGridType::MERCHANT_RESOURCE;
						TempResource->XPos = x;
						TempResource->YPos = y;
						WorldArray[x][y]->ResourceAtLocation = TempResource;
						break;
					default:
						break;
				}
			}
		}
	}

	// Generate Initial Agent Positions
	if(ShipBlueprint)
	{
		for (int i = 0; i < NUM_AGENTS; i++)
		{
			int RandXPos = 0;
			int RandYPos = 0;
			bool isFree = false;

			while (!isFree) {
				RandXPos = FMath::RandRange(0, MapSizeX - 1);
				RandYPos = FMath::RandRange(0, MapSizeY - 1);

				if (WorldArray[RandXPos][RandYPos]->GridType == EGridType::DEEP_WATER && WorldArray[RandXPos][RandYPos]->ObjectAtLocation == nullptr)
				{
					isFree = true;
				}
			}

			FVector Position(RandXPos * GRID_SIZE_WORLD, RandYPos * GRID_SIZE_WORLD, 20);
			AShip* Agent = World->SpawnActor<AShip>(ShipBlueprint, Position, FRotator::ZeroRotator);
			Agent->AddTickPrerequisiteActor(this);
			Agent->Level = this;

			Agent->XPos = RandXPos;
			Agent->YPos = RandYPos;
			
			ShipFleet.Add(Agent);
			WorldArray[RandXPos][RandYPos]->ObjectAtLocation = Agent;
		}
	}

	if (GoldBlueprint)
	{
		for(int i = 0; i < NUM_FOOD; i++)
		{
			int RandXPos = 0;
			int RandYPos = 0;
			bool isFree = false;

			while (!isFree) {
				RandXPos = FMath::RandRange(0, MapSizeX - 1);
				RandYPos = FMath::RandRange(0, MapSizeY - 1);

				if (WorldArray[RandXPos][RandYPos]->GridType == EGridType::DEEP_WATER && WorldArray[RandXPos][RandYPos]->ObjectAtLocation == nullptr)
				{
					isFree = true;
				}
			}

			FVector Position(RandXPos * GRID_SIZE_WORLD, RandYPos * GRID_SIZE_WORLD, 20);
			AGold* NewGold = World->SpawnActor<AGold>(GoldBlueprint, Position, FRotator::ZeroRotator);

			WorldArray[RandXPos][RandYPos]->ResourceAtLocation = NewGold;
			GoldActors.Add(NewGold);
		}
	}

	// Set Static Camera Position
	if(Camera)
	{
		FVector CameraPosition = Camera->GetActorLocation();
		
		CameraPosition.X = MapSizeX * 0.5 * GRID_SIZE_WORLD;
		CameraPosition.Y = MapSizeY * 0.5 * GRID_SIZE_WORLD;
		
		Camera->SetActorLocation(CameraPosition);
	}
}

void ALevelGenerator::SpawnNextGold()
{
	UWorld* World = GetWorld();
	// Generate next Gold Position
	if (GoldBlueprint)
	{
		int RandXPos = 0;
		int RandYPos = 0;
		bool isFree = false;

		while (!isFree) {
			RandXPos = FMath::RandRange(0, MapSizeX - 1);
			RandYPos = FMath::RandRange(0, MapSizeY - 1);

			if (WorldArray[RandXPos][RandYPos]->GridType == EGridType::DEEP_WATER && WorldArray[RandXPos][RandYPos]->ObjectAtLocation == nullptr)
			{
				isFree = true;
			}
		}

		FVector Position(RandXPos * GRID_SIZE_WORLD, RandYPos * GRID_SIZE_WORLD, 20);
		AGold* NewGold = World->SpawnActor<AGold>(GoldBlueprint, Position, FRotator::ZeroRotator);

		WorldArray[RandXPos][RandYPos]->ResourceAtLocation = NewGold;
		GoldActors.Add(NewGold);
	}
}

// Generates the grid of nodes used for pathfinding and also for placement of objects in the game world
void ALevelGenerator::GenerateNodeGrid(char Grid[MAX_MAP_SIZE][MAX_MAP_SIZE])
{
	for(int X = 0; X < MapSizeX; X++)
	{
		for(int Y = 0; Y < MapSizeY; Y++)
		{
			WorldArray[X][Y] = new GridNode();
			WorldArray[X][Y]->X = X;
			WorldArray[X][Y]->Y = Y;
			WorldArray[X][Y]->ResourceNum = 0;
			WorldArray[X][Y]->ObjectAtLocation = nullptr;
			WorldArray[X][Y]->ResourceAtLocation = nullptr;

			// Characters as defined from the map file
			switch(Grid[X][Y])
			{
				case '.':
					WorldArray[X][Y]->GridType = EGridType::DEEP_WATER;
					break;
				case '@':
					WorldArray[X][Y]->GridType = EGridType::LAND;
					break;
				case 'H':
					WorldArray[X][Y]->GridType = EGridType::HOME;
					break;
				case 'W':
					WorldArray[X][Y]->GridType = EGridType::WOOD_RESOURCE;
					WorldArray[X][Y]->ResourceNum = 500;
					break;
				case 'S':
					WorldArray[X][Y]->GridType = EGridType::STONE_RESOURCE;
					WorldArray[X][Y]->ResourceNum = 500;
					break;
				case 'F':
					WorldArray[X][Y]->GridType = EGridType::FRUIT_RESOURCE;
					WorldArray[X][Y]->ResourceNum = 500;
					break;
				case 'M':
					WorldArray[X][Y]->GridType = EGridType::MERCHANT_RESOURCE;
					WorldArray[X][Y]->ResourceNum = 500;
					break;
				default:
					break;
			}
		}
	}
}

// Reset all node values (F, G, H & Parent)
void ALevelGenerator::ResetAllNodes()
{
	for (int X = 0; X < MapSizeX; X++)
	{
		for (int Y = 0; Y < MapSizeY; Y++)
		{
			WorldArray[X][Y]->F = 0;
			WorldArray[X][Y]->G = 0;
			WorldArray[X][Y]->H = 0;
			WorldArray[X][Y]->Parent = nullptr;
			WorldArray[X][Y]->IsChecked = false;
		}
	}
}

float ALevelGenerator::CalculateDistanceBetween(GridNode* first, GridNode* second)
{
	FVector distToTarget = FVector(second->X - first->X,
		second->Y - first->Y, 0);
	return distToTarget.Size();
}

void ALevelGenerator::CalculatePath(AShip* TargetShip, GridNode* GoalNode)
{
	GridNode* currentNode = nullptr;
	GridNode* tempNode = nullptr;
	bool isGoalFound = false;
	
	int SearchCount = 0;

	TQueue<GridNode*> nodesToVisit;

	GridNode* StartNode = WorldArray[TargetShip->XPos][TargetShip->YPos];
	if(!GoalNode) {
		GoalNode = CalculateNearestGoal(StartNode->X, StartNode->Y, EGridType::GOLD_RESOURCE);
	}

	int StartXPos = TargetShip->GetActorLocation().X / GRID_SIZE_WORLD;
	int StartYPos = TargetShip->GetActorLocation().Y / GRID_SIZE_WORLD;
	StartNode = WorldArray[StartXPos][StartYPos];
	StartNode->IsChecked = true;
	nodesToVisit.Enqueue(StartNode);

	while (!nodesToVisit.IsEmpty())
	{
		SearchCount++;
		nodesToVisit.Dequeue(currentNode);

		if (currentNode == GoalNode)
		{
			isGoalFound = true;
			break;
		}

		// Check the left neighbour
		// Check to ensure not out of range
		if (currentNode->Y - 1 > 0)
		{
			// Get the Left neighbor from the list
			tempNode = WorldArray[currentNode->X][currentNode->Y - 1];
			// Check to make sure the node hasnt been visited AND is not closed (A wall)
			if (tempNode->GridType != EGridType::LAND && !tempNode->IsChecked)
			{
				tempNode->IsChecked = true;
				tempNode->Parent = currentNode;
				nodesToVisit.Enqueue(tempNode);
			}
		}

		// Check the top neighbour
		// Check to ensure not out of range
		if (currentNode->X + 1 < MapSizeX)
		{
			// Get the top neighbor from the list
			tempNode = WorldArray[currentNode->X + 1][currentNode->Y];
			// Check to make sure the node hasnt been visited AND is not closed (A wall)
			if (tempNode->GridType != EGridType::LAND && !tempNode->IsChecked)
			{
				tempNode->IsChecked = true;
				tempNode->Parent = currentNode;
				nodesToVisit.Enqueue(tempNode);
			}
		}

		// Check the right neighbour
		// Check to ensure not out of range
		if (currentNode->Y + 1 < MapSizeY)
		{
			// Get the right neighbor from the list
			tempNode = WorldArray[currentNode->X][currentNode->Y + 1];
			// Check to make sure the node hasnt been visited AND is not closed (A wall)
			if (tempNode->GridType != EGridType::LAND && !tempNode->IsChecked)
			{
				tempNode->IsChecked = true;
				tempNode->Parent = currentNode;
				nodesToVisit.Enqueue(tempNode);
			}
		}

		// Check the bottom neighbour
		// Check to ensure not out of range
		if (currentNode->X - 1 > 0)
		{
			// Get the bottom neighbor from the list
			tempNode = WorldArray[currentNode->X - 1][currentNode->Y];
			// Check to make sure the node hasnt been visited AND is not closed (A wall)
			if (tempNode->GridType != EGridType::LAND && !tempNode->IsChecked)
			{
				tempNode->IsChecked = true;
				tempNode->Parent = currentNode;
				nodesToVisit.Enqueue(tempNode);
			}
		}
	}

	if (isGoalFound)
	{
		RenderPath(TargetShip, GoalNode);
		TargetShip->GeneratePath = false;
	}
}

void ALevelGenerator::CalculatePath(AShip* TargetShip, EGridType ResourceType)
{
	GridNode* StartNode = WorldArray[TargetShip->XPos][TargetShip->YPos];
	GridNode* GoalNode = CalculateNearestGoal(StartNode->X, StartNode->Y, ResourceType);

	CalculatePath(TargetShip, GoalNode);
}

void ALevelGenerator::ResetPath(AShip* CurrentShip, GridNode* StartNode, GridNode* GoalNode)
{
	for (int i = 0; i < CurrentShip->PathDisplayActors.Num(); i++)
	{
		CurrentShip->PathDisplayActors[i]->Destroy();
	}
	CurrentShip->PathDisplayActors.Empty();

	CurrentShip->Path.Empty();
}

void ALevelGenerator::DetailPath(AShip* CurrentShip, GridNode* StartNode, GridNode* GoalNode)
{
	/*
	//Onscreen Debug (Don't forget the include!)
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::White, FString::Printf(TEXT("Total Cells searched: %d with a path length of: %d and a distance of: %f"), SearchCount, Ship->Path.Num(), CalculateDistanceBetween(StartNode, GoalNode)));
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::White, FString::Printf(TEXT("The difference between the current implemented path and the direct flight path is: %f"), Ship->Path.Num() / CalculateDistanceBetween(StartNode, GoalNode)));
	//Log Debug message (Accessed through Window->Developer Tools->Output Log)
	UE_LOG(LogTemp, Warning, TEXT("Total Cells searched: %d with a path length of: %d and a distance of: %f"), SearchCount, Ship->Path.Num(), CalculateDistanceBetween(StartNode, GoalNode));
	UE_LOG(LogTemp, Warning, TEXT("The difference between the current implemented path and the direct flight path is: %f"), Ship->Path.Num() / CalculateDistanceBetween(StartNode, GoalNode));
	*/
}

void ALevelGenerator::RenderPath(AShip* CurrentShip, GridNode* GoalNode)
{
	UWorld* World = GetWorld();
	GridNode* CurrentNode = GoalNode;
	while (CurrentNode->Parent != nullptr)
	{
		FVector Position(CurrentNode->X * GRID_SIZE_WORLD, CurrentNode->Y * GRID_SIZE_WORLD, 20);
		AActor* PDActor = World->SpawnActor<AActor>(PathDisplayBlueprint, Position, FRotator::ZeroRotator);
		CurrentShip->PathDisplayActors.Add(PDActor);

		GridNode* tempNode = new GridNode();
		tempNode->X = CurrentNode->X;
		tempNode->Y = CurrentNode->Y;
		
		CurrentShip->Path.EmplaceAt(0,tempNode);
		
		CurrentNode = CurrentNode->Parent;
	}

	ResetAllNodes();
}

GridNode* ALevelGenerator::CalculateNearestGoal(int XPos, int YPos, EGridType ResourceType)
{
	float ShortestPath = 999999;

	if(YPos < 0 || XPos < 0 || YPos >= MapSizeY || XPos >= MapSizeX)
	{
		return nullptr;
	}

	GridNode* CurrentPosition = WorldArray[XPos][YPos];
	
	if(ResourceType == EGridType::GOLD_RESOURCE)
	{
		AGold* NearestGold = nullptr;
		for(auto Gold: GoldActors)
		{
			int xPos = Gold->GetActorLocation().X / GRID_SIZE_WORLD;
			int yPos = Gold->GetActorLocation().Y / GRID_SIZE_WORLD;
			
			float CurrentPath = CalculateDistanceBetween(CurrentPosition, WorldArray[xPos][yPos]);
			if(CurrentPath < ShortestPath)
			{
				NearestGold = Gold;
				ShortestPath = CurrentPath;
			}
		}

		if(NearestGold)
		{
			int xPos = NearestGold->GetActorLocation().X / GRID_SIZE_WORLD;;
			int yPos = NearestGold->GetActorLocation().Y / GRID_SIZE_WORLD;
			return WorldArray[xPos][yPos];
		}

		return nullptr;
	}

	GridNode* GoalNode = nullptr;
	
	for (int X = 0; X < MapSizeX; X++)
	{
		for (int Y = 0; Y < MapSizeY; Y++)
		{
			if(WorldArray[X][Y]->GridType == ResourceType)
			{
				float CurrentPath = CalculateDistanceBetween(CurrentPosition, WorldArray[X][Y]);
				if(CurrentPath < ShortestPath)
				{
					GoalNode = WorldArray[X][Y];
					ShortestPath = CurrentPath;
				}
			}
		}
	}
	
	return GoalNode;
}

GridNode* ALevelGenerator::FindGridNode(AActor* ActorResource)
{
	GridNode* TempNode = nullptr;
	
	for (int X = 0; X < MapSizeX; X++)
	{
		for (int Y = 0; Y < MapSizeY; Y++)
		{
			if(WorldArray[X][Y]->ResourceAtLocation == ActorResource)
			{
				TempNode = WorldArray[X][Y];
			}
		}
	}

	return TempNode;
}


void ALevelGenerator::CollectGold(AGold* CollectedGold)
{
	GoldActors.Remove(CollectedGold);

	for (int X = 0; X < MapSizeX; X++)
	{
		for (int Y = 0; Y < MapSizeY; Y++)
		{
			if(WorldArray[X][Y]->ResourceAtLocation == CollectedGold)
			{
				WorldArray[X][Y]->ResourceAtLocation = nullptr;
			}
		}
	}

	CollectedGold->Destroy();
}

bool ALevelGenerator::IsGoldValid(AGold* CollectedGold)
{
	return GoldActors.Contains(CollectedGold);
}