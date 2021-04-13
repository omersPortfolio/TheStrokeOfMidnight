//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Zoe Purcell

/*Changelog:

	02/17/21
		- Checkpoints added

*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCheckpoint.generated.h"

class UBoxComponent;
class UPGameInstance;

UCLASS()
class THESTROKEOFMIDNIGHT_API APCheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCheckpoint();

	UPROPERTY(EditAnywhere)
	UBoxComponent* CheckpointBox;

	UPGameInstance* GameInstance;

	bool bGameSaved = false;

	UFUNCTION()
	void OnComponentBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
