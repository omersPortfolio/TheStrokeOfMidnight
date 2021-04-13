//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar


#include "Characters/PCharacterBase.h"
#include "Components/PHealthComponent.h"
#include "Framework/PGameInstance.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
APCharacterBase::APCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UPHealthComponent>(TEXT("HealthComponent"));

	Guid.NewGuid();
}

void APCharacterBase::OnDeath()
{

}

void APCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	SpawnLocation = GetActorLocation();

	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Guid.ToString());
}

