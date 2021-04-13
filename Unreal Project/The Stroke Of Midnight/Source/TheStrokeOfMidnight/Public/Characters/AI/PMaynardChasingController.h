// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PMaynardChasingController.generated.h"

/**
 * 
 */
UCLASS()
class THESTROKEOFMIDNIGHT_API APMaynardChasingController : public AAIController
{
	GENERATED_BODY()
	
public:
	APMaynardChasingController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blackboard Keys")
	FName TargetActorKey;


private:
	virtual void BeginPlay() override;

	class APPlayer* Player;
};
