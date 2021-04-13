//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Zoe Purcell and Omer Kocar


#include "Misc/PPortal.h"
#include "Components/BoxComponent.h"
#include "Characters/PPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <Camera/PlayerCameraManager.h>
#include <Camera/CameraComponent.h>
#include "Framework/PGameInstance.h"
#include <Math/UnrealMathUtility.h>

// Sets default values
APPortal::APPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>("Portal Mesh");
	SetRootComponent(PortalMesh);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Trigger Box");
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPortal::OnComponentBeginOverlap);

	//Only used for 2-Way Portals
	ExitOffsetDistance = 0.0f;

	FadeInOutTime = 0.5f;
	TeleportTime = 3.0f;
}

void APPortal::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bTeleportTriggered && WarpActor && ExitActor && OtherActor->IsA(APPlayer::StaticClass()))
	{
		//Applies the offset to prevent an infinite teleportation loop. Won't do anything with 1-way portals.
		ExitOffsetVector = ExitActor->GetActorForwardVector() * ExitOffsetDistance;		

		PlayerController = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld());
		Player = Cast<APPlayer>(OtherActor);
	
		if (PlayerController)
		{
			APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
			Player->DisableInput(PlayerController);
			Player->GetCameraComponent()->bUsePawnControlRotation = false;

			CameraManager->StartCameraFade(0, 1, FadeInOutTime, FColor::Black, true, true);
			CameraManager->PlayCameraShake(CameraShake, 1);

			FTimerDelegate WarpDelegate = FTimerDelegate::CreateUObject(this, &APPortal::Warp, OtherActor);
			GetWorldTimerManager().SetTimer(WarpTimerHandle, WarpDelegate, FadeInOutTime, false);
		}

		bTeleportTriggered = true;
	}
}

#if WITH_EDITOR
void APPortal::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	OffsetTime = FMath::Clamp(OffsetTime, 0.0f, FadeInOutTime - 0.1f);
}

#endif

void APPortal::Warp(AActor* OtherActor)
{
	// Blend the camera to the WarpActor
	PlayerController->SetViewTargetWithBlend(WarpActor, 0);
	PlayerController->PlayerCameraManager->StartCameraFade(1, 0, FadeInOutTime, FColor::Black, true, true);

	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(GetWorld(), LevelToLoad, true, false, LatentInfo);

	// Set timer to fadeout inside the warp
	GetWorldTimerManager().ClearTimer(WarpTimerHandle);
	FTimerDelegate FadeInOutDelegate = FTimerDelegate::CreateUObject(this, &APPortal::FadeInOut, OtherActor);
	GetWorldTimerManager().SetTimer(FadeInOutTimerHandle, FadeInOutDelegate, TeleportTime, false);
}

void APPortal::FadeInOut(AActor* OtherActor)
{
	PlayerController->PlayerCameraManager->StartCameraFade(0, 1, FadeInOutTime, FColor::Black, true, true);

	// Set timer to teleport from warp to Exit Portal
	FTimerDelegate TeleportDelegate = FTimerDelegate::CreateUObject(this, &APPortal::Teleport, OtherActor);
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, TeleportDelegate, FadeInOutTime + OffsetTime, false);
}

void APPortal::Teleport(AActor* OtherActor)
{
	//Teleports the player
	OtherActor->SetActorLocation(ExitActor->GetActorLocation() + ExitOffsetVector);
	//Sets the player's rotation to the Exit's forward direction
	PlayerController->SetControlRotation(ExitActor->GetActorForwardVector().Rotation());

	// Enable Input
	Player->EnableInput(PlayerController);
	Player->GetCameraComponent()->bUsePawnControlRotation = true;

	// Fade and possess the player camera
	PlayerController->PlayerCameraManager->StartCameraFade(1, 0, FadeInOutTime - OffsetTime, FColor::Black, true, true);
	PlayerController->SetViewTargetWithBlend(OtherActor, 0);

	FLatentActionInfo LatentInfo;
	//UGameplayStatics::UnloadStreamLevel(GetWorld(), LevelToUnload, LatentInfo, false);

	UPGameInstance* GameInstance = Cast<UPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->SetLevelName(LevelToLoad.ToString());
	GameInstance->SaveGame();
}
