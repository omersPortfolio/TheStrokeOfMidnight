//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar


#include "Interactables/PInteractableBase.h"
#include <Components/SphereComponent.h>
#include "Characters/PPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include "Framework/PGameInstance.h"

APInteractableBase::APInteractableBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = RootComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComp);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	CollisionBox->SetupAttachment(Mesh);

	Guid.NewGuid();
}

void APInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Find the player
	Character = Cast<APPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

