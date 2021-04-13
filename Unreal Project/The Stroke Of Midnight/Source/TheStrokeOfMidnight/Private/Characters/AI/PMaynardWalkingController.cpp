// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar


#include "Characters/AI/PMaynardWalkingController.h"

void APMaynardWalkingController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunBehaviorTree(BehaviorTree);
}
