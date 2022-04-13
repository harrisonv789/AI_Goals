/**
 * FIT3094 ASSIGNMENT 2 - GOAL PLANNING
 * Author: Harrison Verrios
 */

#include "GameMode/A2_GoalsGameModeBase.h"

#include "Misc/FileHelper.h"

TArray<FString> AA2_GoalsGameModeBase::GetMapArray()
{
	TArray<FString> MapArray;


	//FString MapText = GetMapText(FString MapName);
	const FString MapText = GetRandomMapText();
	MapText.ParseIntoArrayLines(MapArray);

	return MapArray;
}

FString AA2_GoalsGameModeBase::GetRandomMapText()
{
	TArray<FString> MapFiles = GetMapFileList();

	const int32 MapPosition = FMath::RandRange(0, MapFiles.Num() - 1);
	const FString MapPath = MapFiles[MapPosition];

	FString MapText;
	FFileHelper::LoadFileToString(MapText, *MapPath);

	return MapText;
}

TArray<FString> AA2_GoalsGameModeBase::GetMapFileList() {
    TArray<FString> MapFiles;

    const FString MapsDir = FPaths::ProjectContentDir() + "MapFiles/";
    FPlatformFileManager::Get().GetPlatformFile().FindFiles(MapFiles, *MapsDir, nullptr);
	    
    return MapFiles;
}




