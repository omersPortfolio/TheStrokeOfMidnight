// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

#pragma once

#include "CoreMinimal.h"
#include "Triggers/PTriggerBase.h"
#include "PTriggerBreakable.generated.h"

/**
 * 
 */
UCLASS()
class THESTROKEOFMIDNIGHT_API APTriggerBreakable : public APTriggerBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Breakable")
	float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Breakable", meta = (ClampMin = "0.01"))
	float ExplodeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Breakable")
	FVector Impulse;

	FTimerHandle TimerHandle;

	FTimerHandle ListTimer;

	void Explode(AActor* OverlappedActor, AActor* OtherActor);

	TArray<class UDestructibleMesh*> DestructibleMeshes;

private:
	void Event(AActor* OverlappedActor, AActor* OtherActor) override;

	virtual void BeginPlay() override;
};
