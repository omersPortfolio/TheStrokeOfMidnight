// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PMaynardWalkingController.generated.h"

/**
 * 
 */
UCLASS()
class THESTROKEOFMIDNIGHT_API APMaynardWalkingController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	UBehaviorTree* BehaviorTree;
	
};
