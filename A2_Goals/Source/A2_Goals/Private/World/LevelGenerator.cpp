/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "World/LevelGenerator.h"
#include "Items/ResourceActor.h"
#include "Engine/World.h"
#include "Agents/Ship.h"

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
void ALevelGenerator::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	bool resetMovement = true;

	for(const auto ship : ShipFleet)
	{
		if(!ship->FinishedMoving)
		{
			resetMovement = false;
		}
	}

	// If resetting its current movement
	if(resetMovement)
	{
		for(const auto ship : ShipFleet)
		{
			ship->FinishedMoving = false;
		}
	}

	// Spawns the next gold
	if(GoldActors.Num() < NUM_FOOD)
	{
		SpawnNextGold();
	}
}

void ALevelGenerator::GenerateWorldFromFile(TArray<FString> worldArray)
{
	// If empty array exit immediately something is horribly wrong
	if(worldArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("World Array is empty!"));
		return;
	}

	// Second line is Height (aka Y value)
	FString heightString = worldArray[1];
	heightString.RemoveFromStart("Height ");
	MapSizeY = FCString::Atoi(*heightString);
	UE_LOG(LogTemp, Warning, TEXT("Height: %d"), MapSizeY);

	// Third line is Width (aka X value)
	FString widthString = worldArray[2];
	widthString.RemoveFromStart("width ");
	MapSizeX = FCString::Atoi(*widthString);
	UE_LOG(LogTemp, Warning, TEXT("Width: %d"), MapSizeX);

	char charMapArray[MAX_MAP_SIZE][MAX_MAP_SIZE];
	
	// Read through the Map section for create the CharMapArray
	for (int lineNum = 4; lineNum < MapSizeY + 4; lineNum++)
	{
		for (int charNum = 0; charNum < worldArray[lineNum].Len(); charNum++)
		{
			charMapArray[lineNum-4][charNum] = worldArray[lineNum][charNum];
		}
	}

	GenerateNodeGrid(charMapArray);
	SpawnWorldActors(charMapArray);
}


void ALevelGenerator::SpawnWorldActors(char grid[MAX_MAP_SIZE][MAX_MAP_SIZE])
{
	UWorld* world = GetWorld();

	// Make sure that all blueprints are connected. If not then fail
	if(DeepBlueprint && ShallowBlueprint && LandBlueprint && WoodBlueprint && StoneBlueprint && FruitBlueprint && MerchantBlueprint)
	{
		AResourceActor* tempResource;
		
		// For each grid space spawn an actor of the correct type in the game world
		for(int x = 0; x < MapSizeX; x++)
		{
			for (int y = 0; y < MapSizeY; y++)
			{
				const float xPos = x * GRID_SIZE_WORLD;
				const float yPos = y * GRID_SIZE_WORLD;

				FVector position(xPos, yPos, 0);

				switch (grid[x][y])
				{
					case '.':
						world->SpawnActor(DeepBlueprint, &position, &FRotator::ZeroRotator);
						break;
					case '@':
						world->SpawnActor(LandBlueprint, &position, &FRotator::ZeroRotator);
						break;
					case 'H':
						world->SpawnActor(ShallowBlueprint, &position, &FRotator::ZeroRotator);
						break;
					case 'W':
						world->SpawnActor(WoodBlueprint, &position, &FRotator::ZeroRotator);
						tempResource = Cast<AResourceActor>(world->SpawnActor(ResourceBlueprint, &position, &FRotator::ZeroRotator));
						tempResource->ResourceType = EGridType::WOOD_RESOURCE;
						tempResource->XPos = x;
						tempResource->YPos = y;
						WorldArray[x][y]->ResourceAtLocation = tempResource;
						break;
					case 'S':
						world->SpawnActor(StoneBlueprint, &position, &FRotator::ZeroRotator);
						tempResource = Cast<AResourceActor>(world->SpawnActor(ResourceBlueprint, &position, &FRotator::ZeroRotator));
						tempResource->ResourceType = EGridType::STONE_RESOURCE;
						tempResource->XPos = x;
						tempResource->YPos = y;
						WorldArray[x][y]->ResourceAtLocation = tempResource;
						break;
					case 'F':
						world->SpawnActor(FruitBlueprint, &position, &FRotator::ZeroRotator);
						tempResource = Cast<AResourceActor>(world->SpawnActor(ResourceBlueprint, &position, &FRotator::ZeroRotator));
						tempResource->ResourceType = EGridType::FRUIT_RESOURCE;
						tempResource->XPos = x;
						tempResource->YPos = y;
						WorldArray[x][y]->ResourceAtLocation = tempResource;
						break;
					case 'M':
						world->SpawnActor(MerchantBlueprint, &position, &FRotator::ZeroRotator);
						tempResource = Cast<AResourceActor>(world->SpawnActor(ResourceBlueprint, &position, &FRotator::ZeroRotator));
						tempResource->ResourceType = EGridType::MERCHANT_RESOURCE;
						tempResource->XPos = x;
						tempResource->YPos = y;
						WorldArray[x][y]->ResourceAtLocation = tempResource;
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
			int randXPos = 0;
			int randYPos = 0;
			bool isFree = false;

			while (!isFree) {
				randXPos = FMath::RandRange(0, MapSizeX - 1);
				randYPos = FMath::RandRange(0, MapSizeY - 1);

				if (WorldArray[randXPos][randYPos]->GridType == EGridType::DEEP_WATER && WorldArray[randXPos][randYPos]->ObjectAtLocation == nullptr)
				{
					isFree = true;
				}
			}

			FVector position(randXPos * GRID_SIZE_WORLD, randYPos * GRID_SIZE_WORLD, 20);
			AShip* agent = world->SpawnActor<AShip>(ShipBlueprint, position, FRotator::ZeroRotator);
			agent->AddTickPrerequisiteActor(this);
			agent->Level = this;

			agent->XPos = randXPos;
			agent->YPos = randYPos;
			
			ShipFleet.Add(agent);
			WorldArray[randXPos][randYPos]->ObjectAtLocation = agent;
		}
	}

	if (GoldBlueprint)
	{
		for(int i = 0; i < NUM_FOOD; i++)
		{
			int randXPos = 0;
			int randYPos = 0;
			bool isFree = false;

			while (!isFree) {
				randXPos = FMath::RandRange(0, MapSizeX - 1);
				randYPos = FMath::RandRange(0, MapSizeY - 1);

				if (WorldArray[randXPos][randYPos]->GridType == EGridType::DEEP_WATER && WorldArray[randXPos][randYPos]->ObjectAtLocation == nullptr)
				{
					isFree = true;
				}
			}

			FVector position(randXPos * GRID_SIZE_WORLD, randYPos * GRID_SIZE_WORLD, 20);
			AGold* newGold = world->SpawnActor<AGold>(GoldBlueprint, position, FRotator::ZeroRotator);

			WorldArray[randXPos][randYPos]->ResourceAtLocation = newGold;
			GoldActors.Add(newGold);
		}
	}

	// Set Static Camera Position
	if(Camera)
	{
		FVector cameraPosition = Camera->GetActorLocation();
		
		cameraPosition.X = MapSizeX * 0.5 * GRID_SIZE_WORLD;
		cameraPosition.Y = MapSizeY * 0.5 * GRID_SIZE_WORLD;
		
		Camera->SetActorLocation(cameraPosition);
	}
}


void ALevelGenerator::SpawnNextGold()
{
	UWorld* world = GetWorld();
	
	// Generate next Gold Position
	if (GoldBlueprint)
	{
		int randXPos = 0;
		int randYPos = 0;
		bool isFree = false;

		while (!isFree) {
			randXPos = FMath::RandRange(0, MapSizeX - 1);
			randYPos = FMath::RandRange(0, MapSizeY - 1);

			if (WorldArray[randXPos][randYPos]->GridType == EGridType::DEEP_WATER && WorldArray[randXPos][randYPos]->ObjectAtLocation == nullptr)
			{
				isFree = true;
			}
		}

		const FVector position(randXPos * GRID_SIZE_WORLD, randYPos * GRID_SIZE_WORLD, 20);
		AGold* newGold = world->SpawnActor<AGold>(GoldBlueprint, position, FRotator::ZeroRotator);

		WorldArray[randXPos][randYPos]->ResourceAtLocation = newGold;
		GoldActors.Add(newGold);
	}
}


// Generates the grid of nodes used for pathfinding and also for placement of objects in the game world
void ALevelGenerator::GenerateNodeGrid(char grid[MAX_MAP_SIZE][MAX_MAP_SIZE])
{
	for(int x = 0; x < MapSizeX; x++)
	{
		for(int y = 0; y < MapSizeY; y++)
		{
			WorldArray[x][y] = new GridNode();
			WorldArray[x][y]->X = x;
			WorldArray[x][y]->Y = y;
			WorldArray[x][y]->ResourceNum = 0;
			WorldArray[x][y]->ObjectAtLocation = nullptr;
			WorldArray[x][y]->ResourceAtLocation = nullptr;

			// Characters as defined from the map file
			switch(grid[x][y])
			{
				case '.':
					WorldArray[x][y]->GridType = EGridType::DEEP_WATER;
					break;
				case '@':
					WorldArray[x][y]->GridType = EGridType::LAND;
					break;
				case 'H':
					WorldArray[x][y]->GridType = EGridType::HOME;
					break;
				case 'W':
					WorldArray[x][y]->GridType = EGridType::WOOD_RESOURCE;
					WorldArray[x][y]->ResourceNum = 500;
					break;
				case 'S':
					WorldArray[x][y]->GridType = EGridType::STONE_RESOURCE;
					WorldArray[x][y]->ResourceNum = 500;
					break;
				case 'F':
					WorldArray[x][y]->GridType = EGridType::FRUIT_RESOURCE;
					WorldArray[x][y]->ResourceNum = 500;
					break;
				case 'M':
					WorldArray[x][y]->GridType = EGridType::MERCHANT_RESOURCE;
					WorldArray[x][y]->ResourceNum = 500;
					break;
				default:
					break;
			}
		}
	}
}


// Reset all node values (F, G, H & Parent)
void ALevelGenerator::ResetAllNodes() const
{
	for (int x = 0; x < MapSizeX; x++)
	{
		for (int y = 0; y < MapSizeY; y++)
		{
			WorldArray[x][y]->F = 0;
			WorldArray[x][y]->G = 0;
			WorldArray[x][y]->H = 0;
			WorldArray[x][y]->Parent = nullptr;
			WorldArray[x][y]->IsChecked = false;
		}
	}
}


float ALevelGenerator::CalculateDistanceBetween(GridNode* first, GridNode* second) const
{
	const FVector distToTarget = FVector(second->X - first->X,second->Y - first->Y, 0);
	return distToTarget.Size();
}


void ALevelGenerator::CalculatePath(AShip* targetShip, GridNode* goalNode)
{
	GridNode* currentNode = nullptr;
	GridNode* tempNode = nullptr;
	bool isGoalFound = false;
	
	int searchCount = 0;

	TQueue<GridNode*> nodesToVisit;

	GridNode* startNode = WorldArray[targetShip->XPos][targetShip->YPos];
	if(!goalNode) {
		goalNode = CalculateNearestGoal(startNode->X, startNode->Y, EGridType::GOLD_RESOURCE);
	}

	const int startXPos = targetShip->GetActorLocation().X / GRID_SIZE_WORLD;
	const int startYPos = targetShip->GetActorLocation().Y / GRID_SIZE_WORLD;
	startNode = WorldArray[startXPos][startYPos];
	startNode->IsChecked = true;
	nodesToVisit.Enqueue(startNode);

	while (!nodesToVisit.IsEmpty())
	{
		searchCount++;
		nodesToVisit.Dequeue(currentNode);

		if (currentNode == goalNode)
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
			// Check to make sure the node hasn't been visited AND is not closed (A wall)
			if (tempNode->GridType != LAND && !tempNode->IsChecked)
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
			// Check to make sure the node hasn't been visited AND is not closed (A wall)
			if (tempNode->GridType != LAND && !tempNode->IsChecked)
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
			// Check to make sure the node hasn't been visited AND is not closed (A wall)
			if (tempNode->GridType != LAND && !tempNode->IsChecked)
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
			// Check to make sure the node hasn't been visited AND is not closed (A wall)
			if (tempNode->GridType != LAND && !tempNode->IsChecked)
			{
				tempNode->IsChecked = true;
				tempNode->Parent = currentNode;
				nodesToVisit.Enqueue(tempNode);
			}
		}
	}

	if (isGoalFound)
	{
		RenderPath(targetShip, goalNode);
		targetShip->GeneratePath = false;
	}
}


void ALevelGenerator::CalculatePath(AShip* targetShip, EGridType resourceType)
{
	const GridNode* startNode = WorldArray[targetShip->XPos][targetShip->YPos];
	GridNode* goalNode = CalculateNearestGoal(startNode->X, startNode->Y, resourceType);

	CalculatePath(targetShip, goalNode);
}


void ALevelGenerator::ResetPath(AShip* currentShip, GridNode* startNode, GridNode* goalNode) const
{
	for (int i = 0; i < currentShip->PathDisplayActors.Num(); i++)
	{
		currentShip->PathDisplayActors[i]->Destroy();
	}
	currentShip->PathDisplayActors.Empty();

	currentShip->Path.Empty();
}


void ALevelGenerator::DetailPath(AShip* currentShip, GridNode* startNode, GridNode* goalNode) const
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


void ALevelGenerator::RenderPath(AShip* currentShip, const GridNode* goalNode) const
{
	UWorld* world = GetWorld();
	const GridNode* currentNode = goalNode;
	
	while (currentNode->Parent != nullptr)
	{
		FVector position(currentNode->X * GRID_SIZE_WORLD, currentNode->Y * GRID_SIZE_WORLD, 20);
		AActor* pdActor = world->SpawnActor<AActor>(PathDisplayBlueprint, position, FRotator::ZeroRotator);
		currentShip->PathDisplayActors.Add(pdActor);

		GridNode* tempNode = new GridNode();
		tempNode->X = currentNode->X;
		tempNode->Y = currentNode->Y;
		
		currentShip->Path.EmplaceAt(0,tempNode);
		
		currentNode = currentNode->Parent;
	}

	ResetAllNodes();
}


GridNode* ALevelGenerator::CalculateNearestGoal(int xPos, int yPos, EGridType resourceType)
{
	float shortestPath = 999999;

	if(yPos < 0 || xPos < 0 || yPos >= MapSizeY || xPos >= MapSizeX)
	{
		return nullptr;
	}

	GridNode* currentPosition = WorldArray[xPos][yPos];
	
	if(resourceType == EGridType::GOLD_RESOURCE)
	{
		AGold* nearestGold = nullptr;
		for(const auto gold: GoldActors)
		{
			const int goldXPos = gold->GetActorLocation().X / GRID_SIZE_WORLD;
			const int goldYPos = gold->GetActorLocation().Y / GRID_SIZE_WORLD;
			
			const float currentPath = CalculateDistanceBetween(currentPosition, WorldArray[goldXPos][goldYPos]);
			if(currentPath < shortestPath)
			{
				nearestGold = gold;
				shortestPath = currentPath;
			}
		}

		if(nearestGold)
		{
			const int goldXPos = nearestGold->GetActorLocation().X / GRID_SIZE_WORLD;;
			const int goldYPos = nearestGold->GetActorLocation().Y / GRID_SIZE_WORLD;
			return WorldArray[goldXPos][goldYPos];
		}

		return nullptr;
	}

	GridNode* goalNode = nullptr;
	
	for (int x = 0; x < MapSizeX; x++)
	{
		for (int y = 0; y < MapSizeY; y++)
		{
			if(WorldArray[x][y]->GridType == resourceType)
			{
				const float currentPath = CalculateDistanceBetween(currentPosition, WorldArray[x][y]);
				if(currentPath < shortestPath)
				{
					goalNode = WorldArray[x][y];
					shortestPath = currentPath;
				}
			}
		}
	}
	
	return goalNode;
}


GridNode* ALevelGenerator::FindGridNode(AActor* actorResource) const
{
	GridNode* tempNode = nullptr;
	
	for (int x = 0; x < MapSizeX; x++)
	{
		for (int y = 0; y < MapSizeY; y++)
		{
			if(WorldArray[x][y]->ResourceAtLocation == actorResource)
			{
				tempNode = WorldArray[x][y];
			}
		}
	}

	return tempNode;
}


void ALevelGenerator::CollectGold(AGold* CollectedGold)
{
	GoldActors.Remove(CollectedGold);

	for (int x = 0; x < MapSizeX; x++)
	{
		for (int y = 0; y < MapSizeY; y++)
		{
			if(WorldArray[x][y]->ResourceAtLocation == CollectedGold)
			{
				WorldArray[x][y]->ResourceAtLocation = nullptr;
			}
		}
	}

	CollectedGold->Destroy();
}


bool ALevelGenerator::IsGoldValid(AGold* collectedGold) const
{
	return GoldActors.Contains(collectedGold);
}