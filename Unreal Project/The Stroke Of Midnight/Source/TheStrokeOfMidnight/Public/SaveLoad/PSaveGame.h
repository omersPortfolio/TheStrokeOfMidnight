// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

/*Changelog:

	01/30/21
		- Created the class.
	03/19/21
		- Created the Data variables.
	03/22/21
		- Created TransformData variable.
	03/24/21
		- Created TriggerData, DoorData and MaynardData
	03/29/21
		- Cleanups

*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PSaveData.h"
#include "PSaveGame.generated.h"

UCLASS()
class THESTROKEOFMIDNIGHT_API UPSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPSaveGame();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPlayerSaveData PlayerData;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FMaynardSaveData MaynardData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FStatueSaveData StatueData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FInteractableSaveData InteractableData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FTriggerSaveData TriggerData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLevelSaveData LevelData;
};
