//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar


#include "Triggers/PTriggerBase.h"
#include <Kismet/GameplayStatics.h>

#include <Components/TextRenderComponent.h>
#include "Framework/PGameInstance.h"
#include <Components/ShapeComponent.h>
#include <Camera/PlayerCameraManager.h>
#include "Characters/PPlayerController.h"

APTriggerBase::APTriggerBase()
{
	// Temporary
	Text = CreateDefaultSubobject<UTextRenderComponent>("Text");
	Text->SetupAttachment(RootComponent);
	Text->SetHorizontalAlignment(EHTA_Center);
	Text->SetRelativeLocation(FVector(0,0, 40));
	Text->SetTextRenderColor(FColor::Black);

	OnActorBeginOverlap.AddDynamic(this, &APTriggerBase::Event);

	bActorSelect = false;

	NoiseLoudness = 1.0f;
	NoiseRange = 500.0f;
	NoiseTag = NAME_None;

	ParticlePreSpawnTime = 0.085f;
	SoundPrePlayTime = 0.085f;
	DestroySoundVolumeMultiplier = 1;

	Guid.NewGuid();
}

void APTriggerBase::BeginPlay()
{
	Super::BeginPlay();

	// Find the Player.
	Character = Cast<APPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void APTriggerBase::Event(class AActor* OverlappedActor, class AActor* OtherActor)
{	
	if (!bTriggered)
	{
		bTriggered = true;
		SetActorHiddenInGame(true);
		// if the overlapped actor is Player
		if (OtherActor == Character)
		{
			// If a trigger sound is selected.
			if (TriggerSound)
			{
				// If Sound Actor is valid, Play sound at its location, otherwise play sound at this trigger box's location.
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), TriggerSound, SoundActor ? SoundActor->GetActorLocation() : GetActorLocation(), VolumeMultiplier);
			}

			// if we want to make noise to trigger AI's hearing stimuli.
			if (bMakesNoise)
			{
				MakeNoise(NoiseLoudness, Character, GetActorLocation(), NoiseRange, NoiseTag);
			}

			// If we want to spawn and SpawnOnTriggerEnd is false, which is only applicable to LookAtTrigger. TODO: will remove bSpawnOnTriggerEnd from here.
			if (bWillSpawn && !bSpawnOnTriggerEnd)
			{
				Spawn();
			}

			if (bPlayCameraShake)
			{
				APPlayerController* PlayerController = Cast<APPlayerController>(GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld()));
				PlayerController->PlayerCameraManager->PlayCameraShake(CameraShake, CameraShakeScale);
			}
		}
	}
	
}

void APTriggerBase::Spawn()
{
	// if a class to spawn is selected.
	if (ClassToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// If bLocationSelect is true, spawn at that location, otherwise spawn at ActorLocationToSpawn's location.
		FVector SpawnLoc = bLocationSelect ? SpawnLocation : ActorLocationToSpawn->GetActorLocation();
		FTransform SpawnTransform(SpawnRotation, SpawnLoc);

		// Spawn the actor.
		AActor* ActorToSpawn = GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTransform, SpawnParams);

		// if we want to destroy the spawned actor
		if (bDestroySpawnedActor)
		{
			// Set the life span of the Spawned Actor to SpawnedActorLifeSpan.
			ActorToSpawn->SetLifeSpan(SpawnedActorLifeSpan);

			if (bSpawnParticle && DestroyParticle)
			{
				FTimerDelegate ParticleDelegate = FTimerDelegate::CreateUObject(this, &APTriggerBase::PlayDestroyEmitter, ActorToSpawn);
				GetWorldTimerManager().SetTimer(ParticleTimerHandle, ParticleDelegate, SpawnedActorLifeSpan - ParticlePreSpawnTime, false);
			}

			if (bPlaySoundOnDestroy && DestroySound)
			{
				FTimerDelegate SoundDelegate = FTimerDelegate::CreateUObject(this, &APTriggerBase::PlayDestroySound, ActorToSpawn);
				GetWorldTimerManager().SetTimer(DestroySoundTimerHandle, SoundDelegate, SpawnedActorLifeSpan - SoundPrePlayTime, false);
			}
		}
	}
}

void APTriggerBase::PlayDestroyEmitter(AActor* ActorToSpawn)
{
	// Spawn Particle Emitter
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyParticle, FVector(ActorToSpawn->GetActorLocation().X, ActorToSpawn->GetActorLocation().Y, -3.0f));
	
}

void APTriggerBase::PlayDestroySound(AActor* ActorToSpawn)
{
	 // Play Sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroySound, ActorToSpawn->GetActorLocation(), DestroySoundVolumeMultiplier);
}

#if WITH_EDITOR
void APTriggerBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (bTriggerOnSight)
	{
		GetCollisionComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	}
	else
	{
		GetCollisionComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}
}
#endif
