// Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar


#include "Characters/AI/PMaynardChasingController.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Characters/PPlayer.h"

APMaynardChasingController::APMaynardChasingController()
{
	TargetActorKey = FName("TargetActor");
}

void APMaynardChasingController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	Player = Cast<APPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (Player)
	{
		GetBlackboardComponent()->SetValueAsObject(TargetActorKey, Player);
	}

	
}
