// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

/*Changelog:

	03/18/21
		- Created the class.
		- Created Save and Load functions.
		- Created the Auto Save feature.

	03/22/21
		- added the feature to save Transforms of the objects in the level.
		- allowed for door rotation reset.
		- removed auto save as it's not needed in our game.
	03/29/21
		- General Cleanups.
		- Fixed Interactable Door save and load bugs.
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PGameInstance.generated.h"

class UPSaveGame;
class APPlayer;
class APAIStatue;
class APAIMaynard;

UCLASS()
class THESTROKEOFMIDNIGHT_API UPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	// Reference to the SaveGame object.
	UPROPERTY(BlueprintReadWrite)
	UPSaveGame* SaveGameInstance;

	// Slot name for save and load
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveLoad")
	FString SaveSlotName;

	UPROPERTY(BlueprintReadOnly, Category = "SaveLoad")
	FString SavedLevelName;

	// Main Save function
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	// Main Load function
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	// Main DeleteSave function
	UFUNCTION(BlueprintCallable)
	void DeleteSaveData();

	UFUNCTION(BlueprintCallable)
	void CreateLoadingScreen();
	UFUNCTION(BlueprintCallable)
	void RemoveLoadingScreen();

	UUserWidget* LoadingScreen;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingScreenClass;

	UFUNCTION(BlueprintCallable)
	void SetLevelName(FString Name);

	FTimerHandle LevelTimerHandle;
	void FinishLoading();

private:
	// Save and Load the properties of every needed object in the level.
	void SaveObjects();
	void LoadObjects();

	// Whether to save / load
	UPROPERTY(EditDefaultsOnly, Category = "SaveLoad")
	bool bCanLoad;

	// Reference to the player
	APPlayer* Player;

	APlayerController* PlayerController;
};
