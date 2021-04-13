//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	03/18/21
		- Created the class.
		- Created FPlayerSaveData
		- Created FDoorSaveData
	03/19/21
		- Created FMaynardSaveData.
		- Created FDoorData
	03/22/21
		- Created FTransformSaveData and its properties.
		- Created Forced Object properties.
	03/29/21 
		- General Cleanups
*/

#pragma once

class APTriggerBase;
class APTriggerObjectMove;
class APAIMaynard;
class APAIStatue;
class APPatrolPath;
class APDoor;
class UDestructibleMesh;
class APInteractableBase;

#include <UObject/ObjectMacros.h>
#include "PSaveData.generated.h"


// Object Transforms
USTRUCT(BlueprintType)
struct FStatueSaveData
{
	GENERATED_BODY()

public:
	FStatueSaveData()
	{
		
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTransform> StatueTransforms;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGuid> StatueIDs;
};

// Player
USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_BODY()
public:
	FPlayerSaveData()
	{
		PlayerTransform = FTransform(FRotator(0, -90, 0), FVector(-100, 6920, -20), FVector(1, 1, 1));
		PlayerRotation = FRotator(0, -90, 0);
	}
	// Player 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform PlayerTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator PlayerRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> PlayerKeyChannels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHoldingLighter;

};

// Trigger Boxes
USTRUCT(BlueprintType)
struct FTriggerSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGuid> TriggerIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTransform> ForcedObjectTransforms;

	// Array that holds the trigger state of trigger boxes.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> bTriggered;
};

// Maynard
USTRUCT(BlueprintType)
struct FMaynardSaveData
{
	GENERATED_BODY()

public:
	// Transform Arrays
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTransform> MaynardTransforms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGuid> MaynardIDs;
	// Maynard's Patrol Datas
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TArray<int> PatrolPathIndices;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TArray<APPatrolPath*> PatrolPaths;
};

// Level
USTRUCT(BlueprintType)
struct FLevelSaveData
{
	GENERATED_BODY()

public:
	// Current Level Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelName;
};

// Door
USTRUCT(BlueprintType)
struct FInteractableSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRotator> DoorRotations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGuid> InteractableDoorIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGuid> InteractableLightIDs;

	// Array that holds the open/close states of the interactable doors
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> bDoorOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> bLightIsOn;
};
