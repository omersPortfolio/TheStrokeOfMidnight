// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar


#include "Characters/AI/PAIMaynard.h"
#include "Characters/PCharacterBase.h"
#include <Components/CapsuleComponent.h>
#include "Characters/PPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>

APAIMaynard::APAIMaynard()
{
	PrimaryActorTick.bCanEverTick = true;

	HandCollider = CreateDefaultSubobject<UCapsuleComponent>("HandCollider");
	HandCollider->SetupAttachment(GetMesh(), FName("LeftHand"));
	HandCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	HandCollider->OnComponentBeginOverlap.AddDynamic(this, &APAIMaynard::BeginOverlap);

	AttackRange = 150.0f;
	Damage = 50.0f;
}

void APAIMaynard::DeactivateMovement()
{
	// Stop movement and deactivate movement.
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->Deactivate();
}

void APAIMaynard::ActivateMovement()
{
	// Activate Character movement
	GetCharacterMovement()->Activate();
}

void APAIMaynard::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// is the overlapping actor Player
	if (OtherActor != this && !bAttacked && OtherActor == Player)
	{
		// Apply Damage to the Player.
		UGameplayStatics::ApplyDamage(Player, Damage, GetInstigatorController(), this, DamageClass);
		bAttacked = true;
	}
}
