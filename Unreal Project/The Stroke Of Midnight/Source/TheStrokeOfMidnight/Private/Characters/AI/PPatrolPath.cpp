// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

#include "Characters/AI/PPatrolPath.h"
#include <Components/SplineComponent.h>

// Sets default values
APPatrolPath::APPatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

	PatrolPath = CreateDefaultSubobject<USplineComponent>("PatrolPath");
	PatrolPath->SetupAttachment(RootComponent);
	PatrolPath->SetDrawDebug(true);
	PatrolPath->SetUnselectedSplineSegmentColor(FLinearColor::Red);
	PatrolPath->SetSelectedSplineSegmentColor(FLinearColor::Black);
}

void APPatrolPath::IncrementPatrolIndex()
{
	// Increment the patrol index by one modulo the total patrol point.
	PatrolIndex = (PatrolIndex + 1) % (PatrolPath->GetNumberOfSplinePoints());
}

FVector APPatrolPath::GetNextPointAsWorldPos()
{
	// return the patrol point as world position.
	return PatrolPath->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}

void APPatrolPath::SetPatrolIndex(int newIndex)
{
	PatrolIndex = newIndex;
}
