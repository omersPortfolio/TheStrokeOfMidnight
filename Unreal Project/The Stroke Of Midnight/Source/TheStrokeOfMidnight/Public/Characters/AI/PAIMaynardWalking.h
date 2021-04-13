// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/AI/INT_AIHelper.h"
#include "PAIMaynardWalking.generated.h"

class APPatrolPath;

UCLASS()
class THESTROKEOFMIDNIGHT_API APAIMaynardWalking : public ACharacter, public IINT_AIHelper
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APAIMaynardWalking();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* MoveToPoint;

};
