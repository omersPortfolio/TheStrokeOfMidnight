// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

#pragma once

#include "CoreMinimal.h"
#include "Characters/AI/PEnemyBase.h"
#include "PAIMaynardChasing.generated.h"

class UCapsuleComponent;

UCLASS()
class THESTROKEOFMIDNIGHT_API APAIMaynardChasing : public APEnemyBase
{
	GENERATED_BODY()

public:
	APAIMaynardChasing();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* HandCollider;

	// Whether Maynard already attacked. This is set to true in BeginOverlap and to false in Attack interface function (see the Blueprint).
	UPROPERTY(BlueprintReadWrite, Category = "NPC")
		bool bAttacked;

private:
	// Maynard's hand's collider to check if it hits the player while attacking.
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
	
};
