//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Zoe Purcell


#include "Characters/AI/PAIStatue.h"
#include "Components/BoxComponent.h"
#include "Characters/PPlayer.h"
#include "Components/AudioComponent.h"
#include "Components/PHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundAttenuation.h"

APAIStatue::APAIStatue()
{
	PrimaryActorTick.bCanEverTick = true;

	KillTrigger = CreateDefaultSubobject<UBoxComponent>("Kill Trigger");
	KillTrigger->SetupAttachment(RootComponent);
	KillTrigger->OnComponentBeginOverlap.AddDynamic(this, &APAIStatue::BeginOverlap);

	// A one-hit kill onto the player
	Damage = 100.0f;

	bUseControllerRotationYaw = false;

	// We want the Statue to move extremely fast towards the player when they're not looking at it
	GetCharacterMovement()->MaxWalkSpeed = 1500;
	GetCharacterMovement()->RotationRate = FRotator(0, 0, 200);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AttenSettings.FalloffDistance = 2000.0f;
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComp->SetupAttachment(RootComponent);
	AudioComp->bOverrideAttenuation = true;
	AudioComp->AttenuationOverrides = AttenSettings;

	LastRotation = GetActorRotation();
}

void APAIStatue::Tick(float DeltaSeconds)
{
	// Only play movement sounds if the Player is alive
	if (Player)
	{
		if (!IINT_AIHelper::Execute_IsDead(Player))
		{
			// Check if Statue is moving
			if (GetVelocity().Size() > 0)
			{
				// If the AudioComponent is currently playing the turning sound, stop it
				if (AudioComp->Sound == TurnSound && AudioComp->IsPlaying())
				{
					AudioComp->FadeOut(0.3f, 0.0f);
				}
				// Play the movement sound
				if (!AudioComp->IsPlaying())
				{
					AudioComp->SetSound(MovementSound);
					AudioComp->Play();
				}
			}
			// If the Statue is no longer moving, stop the movement sound
			else
			{
				if (AudioComp->Sound == MovementSound && AudioComp->IsPlaying())
				{
					AudioComp->FadeOut(0.3f, 0.0f);
				}
			}

			// Check if the Statue is turning.
			// If Our current rotation isn't the same as it was last frame, then we ware currently turning
			// Also only play the turning sound if we aren't moving, in order to prioritize the movement sound effect.
			if (LastRotation != GetActorRotation() && GetVelocity().Size() == 0)
			{
				// If the AudioComponent is currently playing the movement sound, stop it
				if (AudioComp->Sound == MovementSound && AudioComp->IsPlaying())
				{
					AudioComp->FadeOut(0.3f, 0.0f);
				}
				// Play the turning sound
				if (!AudioComp->IsPlaying())
				{
					AudioComp->SetSound(TurnSound);
					AudioComp->Play();
				}
			}
			// If the Statue is no longer turning, fade out the turn sound
			else
			{
				if (AudioComp->Sound == TurnSound && AudioComp->IsPlaying())
				{
					AudioComp->FadeOut(0.3f, 0.0f);
				}
			}
		}
		// Update the last known rotation
		LastRotation = GetActorRotation();
	}
}

void APAIStatue::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Making sure we're overlapping with the Player before we apply damage
	if (OtherActor && OtherActor != this && OtherActor == Player)
	{
		UGameplayStatics::ApplyDamage(Player, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		
		if (AudioComp->IsPlaying())
		{
			AudioComp->FadeOut(0.3f, 0.0f);
		}
	}
}
