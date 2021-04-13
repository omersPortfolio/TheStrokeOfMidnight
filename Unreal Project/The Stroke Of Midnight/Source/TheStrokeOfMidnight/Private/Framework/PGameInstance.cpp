// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar


#include "Framework/PGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include "SaveLoad/PSaveGame.h"
#include "Characters/PPlayer.h"
#include "Characters/PPlayerController.h"
#include "Characters/AI/PAIStatue.h"
#include "Characters/AI/PAIMaynard.h"
#include "Interactables/PInteractableBase.h"
#include "Characters/AI/PPatrolPath.h"
#include "Interactables/PInteractableDoor.h"
#include "Triggers/PTriggerObjectMove.h"
#include "Triggers/PTriggerBase.h"
#include "Triggers/PTriggerBreakable.h"
#include "DestructibleComponent.h"
#include "Interactables/PInteractableLight.h"
#include <Components/PointLightComponent.h>
#include <Blueprint/UserWidget.h>
#include "Characters/PCharacterBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include <Misc/Guid.h>
#include "Components/PHealthComponent.h"


void UPGameInstance::Init()
{
	Super::Init();

	SaveSlotName = "SaveSlot";
}

void UPGameInstance::SaveGame()
{
	Player = Cast<APPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Game is saving...");

		SaveGameInstance = Cast<UPSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		//SaveGameInstance->LevelData.LevelName = SavedLevelName;
		SaveObjects();
		SavedLevelName = SaveGameInstance->LevelData.LevelName;


		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);

		if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, UGameplayStatics::GetCurrentLevelName(GetWorld()) + " saved");
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Game has been successfully saved...");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Saving failed!");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Player is not found!");
	}
}

void UPGameInstance::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SaveGameInstance = Cast<UPSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

		CreateLoadingScreen();

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Game is loading...");
		if (SaveGameInstance->LevelData.LevelName != "")
		{
			FLatentActionInfo LatentInfo;
			UGameplayStatics::LoadStreamLevel(GetWorld(), "Area_1", true, true, LatentInfo);
			UGameplayStatics::UnloadStreamLevel(GetWorld(), "MainMenuMap", LatentInfo, true);
			GetWorld()->GetTimerManager().SetTimer(LevelTimerHandle, this, &UPGameInstance::FinishLoading, 1.0f, false);
		}
		else
		{
			FLatentActionInfo LatentInfo;
			UGameplayStatics::LoadStreamLevel(GetWorld(), "Area_1", true, true, LatentInfo);
			GetWorld()->GetTimerManager().SetTimer(LevelTimerHandle, this, &UPGameInstance::FinishLoading, 1.0f, false);
		}
	}
}

void UPGameInstance::SetLevelName(FString Name)
{
	SaveGameInstance->LevelData.LevelName = Name;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
}

void UPGameInstance::FinishLoading()
{
	Player = Cast<APPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(GetWorld(), *SaveGameInstance->LevelData.LevelName, true, true, LatentInfo);
		Player->OnRespawn();
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->UnPossess();
		PlayerController->Possess(Player);
		RemoveLoadingScreen();

		LoadObjects();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Game has been loaded");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Player is not saved yet!");
	}
}

void UPGameInstance::DeleteSaveData()
{
	UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0);
	if(SaveGameInstance)
		SaveGameInstance = nullptr;
}

void UPGameInstance::CreateLoadingScreen()
{
	if (LoadingScreen)
		LoadingScreen->RemoveFromParent();
	LoadingScreen = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(), 0), LoadingScreenClass);
	LoadingScreen->AddToViewport(100);
}

void UPGameInstance::RemoveLoadingScreen()
{
	if (LoadingScreen)
	{
		LoadingScreen->RemoveFromParent();
		LoadingScreen = nullptr;
	}
}

void UPGameInstance::SaveObjects()
{
	FTransform PlayerTransform = Player->GetTransform();
	FRotator PlayerRotation = Player->PlayerController->GetControlRotation();

	SaveGameInstance->PlayerData.PlayerTransform = PlayerTransform;
	SaveGameInstance->PlayerData.PlayerRotation = PlayerRotation;
	SaveGameInstance->PlayerData.bHoldingLighter = Player->bHoldingLighter;

	// Save Maynards
	TArray<AActor*> FoundMaynards;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APAIMaynard::StaticClass(), FoundMaynards);

	if (FoundMaynards.Num() > 0)
	{
		for (AActor* Actor : FoundMaynards)
		{
			APAIMaynard* Maynard = Cast<APAIMaynard>(Actor);

			SaveGameInstance->MaynardData.MaynardTransforms.Add(Maynard->GetTransform());
			SaveGameInstance->MaynardData.MaynardIDs.Add(Maynard->GetGuid());
			/*if (Maynard->GetPatrol())
			{
				SaveGameInstance->MaynardData.PatrolPathIndices.Add(Maynard->GetPatrol()->PatrolIndex);
			}*/
		}
	}

	// Save Statues
	TArray<AActor*> FoundStatues;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APAIStatue::StaticClass(), FoundStatues);

	if (FoundStatues.Num() > 0)
	{
		for (AActor* Actor : FoundStatues)
		{
			APAIStatue* Statue = Cast<APAIStatue>(Actor);

			SaveGameInstance->StatueData.StatueTransforms.Add(Statue->GetTransform());
			SaveGameInstance->StatueData.StatueIDs.Add(Statue->GetGuid());
		}
	}

	// Save Doors
	TArray<AActor*> FoundDoors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APInteractableDoor::StaticClass(), FoundDoors);

	if (FoundDoors.Num() > 0)
	{
		for (AActor* Actor : FoundDoors)
		{
			APInteractableDoor* Door = Cast<APInteractableDoor>(Actor);
			SaveGameInstance->InteractableData.DoorRotations.Add(Door->Mesh->GetRelativeRotation());
			SaveGameInstance->InteractableData.bDoorOpen.Add(Door->GetOpen());
			SaveGameInstance->InteractableData.InteractableDoorIDs.Add(Door->GetGuid());
		}
	}

	// Save Lights
	TArray<AActor*> FoundLights;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APInteractableLight::StaticClass(), FoundLights);

	if (FoundLights.Num() > 0)
	{
		for (AActor* Actor : FoundLights)
		{
			APInteractableLight* Light = Cast<APInteractableLight>(Actor);
			SaveGameInstance->InteractableData.InteractableLightIDs.Add(Light->GetGuid());
			SaveGameInstance->InteractableData.bLightIsOn.Add(Light->GetState());
		}
	}


	TArray<AActor*> FoundTriggers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APTriggerBase::StaticClass(), FoundTriggers);

	if (FoundTriggers.Num() > 0)
	{
		for (AActor* Actor : FoundTriggers)
		{
			APTriggerBase* Trigger = Cast<APTriggerBase>(Actor);
			SaveGameInstance->TriggerData.TriggerIDs.Add(Trigger->GetGuid());
			SaveGameInstance->TriggerData.bTriggered.Add(Trigger->bTriggered);
		}
	}
}

void UPGameInstance::LoadObjects()
{
	if (Player && Player->PlayerController)
	{
		Player->SetActorTransform(SaveGameInstance->PlayerData.PlayerTransform);
		Player->PlayerController->SetControlRotation(SaveGameInstance->PlayerData.PlayerRotation);
		Player->GetHealthComponent()->CurrentHealth = Player->GetHealthComponent()->MaxHealth;
		if (SaveGameInstance->PlayerData.bHoldingLighter)
			Player->HoldLighter();
	}

	// Load Maynards
	TArray<AActor*> Maynards;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APAIMaynard::StaticClass(), Maynards);
	TArray<FGuid> MaynardIDs = SaveGameInstance->MaynardData.MaynardIDs;

	for (int i = 0; i < Maynards.Num(); i++)
	{
		APAIMaynard* Maynard = Cast<APAIMaynard>(Maynards[i]);
		for (int k = 0; k < MaynardIDs.Num(); k++)
		{
			if (Maynard->GetGuid() == MaynardIDs[i])
			{
				Maynard->SetActorTransform(SaveGameInstance->MaynardData.MaynardTransforms[i]);
				/*if (Maynard->GetPatrol())
				{
					Maynards->GetPatrol()->PatrolIndex = SaveGameInstance->MaynardData.PatrolPathIndices[i];
				}*/
			}
		}
	}

	// Load Statues
	TArray<AActor*> Statues;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APAIStatue::StaticClass(), Statues);
	TArray<FGuid> StatueIDs = SaveGameInstance->StatueData.StatueIDs;
	for (int i = 0; i < Statues.Num(); i++)
	{
		APAIStatue* Statue = Cast<APAIStatue>(Statues[i]);
		for (int k = 0; k < StatueIDs.Num(); k++)
		{
			if (Statue->GetGuid() == StatueIDs[i])
			{
				Statue->SetActorTransform(SaveGameInstance->StatueData.StatueTransforms[i]);
			}
		}
	}

	// Load Interactable Doors
	TArray<AActor*> Doors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APInteractableDoor::StaticClass(), Doors);
	TArray<FGuid> DoorIDs = SaveGameInstance->InteractableData.InteractableDoorIDs;
	
	for (int i = 0; i < Doors.Num(); i++)
	{
		APInteractableDoor* Door = Cast<APInteractableDoor>(Doors[i]);
		for (int k = 0; k < DoorIDs.Num(); k++)
		{
			if (Door->GetGuid() == DoorIDs[i])
			{
				Door->Mesh->SetRelativeRotation(SaveGameInstance->InteractableData.DoorRotations[i]);
				Door->SetOpen(SaveGameInstance->InteractableData.bDoorOpen[i]);

				if (Door->GetOpen() == true)
				{
					Door->GetTimeline().Stop();
				}
			}
		}
	}

	// Load Interactable Lights
	TArray<AActor*> Lights;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APInteractableLight::StaticClass(), Lights);
	TArray<FGuid> LightIDs = SaveGameInstance->InteractableData.InteractableLightIDs;

	for (int i = 0; i < Lights.Num(); i++)
	{
		APInteractableLight* Light = Cast<APInteractableLight>(Lights[i]);
		for (int k = 0; k < LightIDs.Num(); k++)
		{
			if (Light->GetGuid() == LightIDs[i])
			{
				Light->SetState(SaveGameInstance->InteractableData.bLightIsOn[i]);
			}
		}
	}

	/////// Triggers //////////
	// This is so the player can trigger the same trigger box if it is back again.
	Player->TempTrigger = nullptr;

	TArray<AActor*> Triggers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APTriggerBase::StaticClass(), Triggers);
	TArray<FGuid> TriggerIDs = SaveGameInstance->TriggerData.TriggerIDs;
	// Loop through all triggers
	for (int i = 0; i < Triggers.Num(); i++)
	{
		APTriggerBase* Trigger = Cast<APTriggerBase>(Triggers[i]);
		for (int k = 0; k < TriggerIDs.Num(); k++)
		{
			if (Trigger->GetGuid() == TriggerIDs[i])
			{
				// Keep the bTriggered boolean from before saving.
				Trigger->bTriggered = SaveGameInstance->TriggerData.bTriggered[i];

				// if the trigger box was triggered before saving, keep it triggered and hidden. And vice versa.
				Trigger->SetActorHiddenInGame(Trigger->bTriggered);

				// if the found trigger is ObjectMoveTrigger
				if (APTriggerObjectMove* ObjectMoveTrigger = Cast<APTriggerObjectMove>(Trigger))
				{
					for (int j = 0; j < ObjectMoveTrigger->GetActors().Num(); j++)
					{
						AActor* ForcedActor = ObjectMoveTrigger->GetActors()[j];

						// Load the transform of the forced actors from before the save.
						ForcedActor->SetActorTransform(ObjectMoveTrigger->ForcedActorTransforms[j]);

						UMeshComponent* MeshComp = ForcedActor->FindComponentByClass<UMeshComponent>();
						if (MeshComp)
						{
							MeshComp->SetSimulatePhysics(false);
						}
					}
				}

				// Apex Destruction is bugged when loading. will find a way to fix it later.
				/*if (Cast<APTriggerBreakable>(Triggers[i]))
				{
					APTriggerBreakable* BreakableTrigger = Cast<APTriggerBreakable>(Triggers[i]);

					for (int k = 0; k < BreakableTrigger->DestructibleMeshes.Num(); k++)
					{
						AActor* Breakable = BreakableTrigger->GetActors()[k];

						UDestructibleComponent* DestructibleComp = Cast<UDestructibleComponent>(Breakable->GetComponentByClass(UDestructibleComponent::StaticClass()));

						if (!Triggers[k]->bTriggered)
						{
							DestructibleComp->SetDestructibleMesh(BreakableTrigger->DestructibleMeshes[k]);
						}
					}
				}*/
			}
		}
		
	}
}