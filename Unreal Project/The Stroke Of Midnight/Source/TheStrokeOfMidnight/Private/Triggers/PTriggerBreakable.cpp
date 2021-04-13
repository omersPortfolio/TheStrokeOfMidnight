// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

#include "Triggers/PTriggerBreakable.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/DamageType.h>
#include <Components/MeshComponent.h>
#include "DestructibleComponent.h"


void APTriggerBreakable::Event(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!bTriggered)
	{
		// if the overlapped actor is Player.
		if (OtherActor == Character && OtherActor != this)
		{
			FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &APTriggerBreakable::Explode, OverlappedActor, OtherActor);
			GetWorldTimerManager().SetTimer(TimerHandle, RespawnDelegate, ExplodeTimer, false);
		}
	}
}

void APTriggerBreakable::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* Actor : Actors)
	{
		UDestructibleComponent* DestructibleComp = Cast<UDestructibleComponent>(Actor->GetComponentByClass(UDestructibleComponent::StaticClass()));
		if (DestructibleComp)
		{
			DestructibleMeshes.Add(DestructibleComp->GetDestructibleMesh());
		}
	}
}

void APTriggerBreakable::Explode(AActor* OverlappedActor, AActor* OtherActor)
{
	for (AActor* Actor : Actors)
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 100, Actor->GetActorLocation(), DamageRadius, UDamageType::StaticClass(), IgnoredActors);
		UMeshComponent* Mesh;
		Mesh = Cast<UMeshComponent>(Actor->GetComponentByClass(UMeshComponent::StaticClass()));
		Mesh->AddImpulseAtLocation(Impulse * 500.0f, Actor->GetActorLocation());
	}

	Super::Event(OverlappedActor, OtherActor);
}
