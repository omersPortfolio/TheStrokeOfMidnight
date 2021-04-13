//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Zoe Purcell and Omer Kocar.


#include "Misc/PCheckpoint.h"
#include "Components/BoxComponent.h"
#include "Characters/PPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Framework/PGameInstance.h"

// Sets default values
APCheckpoint::APCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the box component
	CheckpointBox = CreateDefaultSubobject<UBoxComponent>("CheckpointBox");
	CheckpointBox->OnComponentBeginOverlap.AddDynamic(this, &APCheckpoint::OnComponentBeginOverlap);
}
void APCheckpoint::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if the overlapped actor is Player
	if (OtherActor->IsA(APPlayer::StaticClass()) && !bGameSaved)
	{
		// Save Game
		GameInstance = Cast<UPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		GameInstance->SaveGame();

		bGameSaved = true;
	}
}