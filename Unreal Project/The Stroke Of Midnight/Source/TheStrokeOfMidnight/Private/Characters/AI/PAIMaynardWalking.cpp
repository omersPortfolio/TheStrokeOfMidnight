// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

#include "Characters/AI/PAIMaynardWalking.h"

// Sets default values
APAIMaynardWalking::APAIMaynardWalking()
{
    MoveToPoint = CreateDefaultSubobject<USceneComponent>("MoveToPoint");
    MoveToPoint->SetupAttachment(RootComponent);
    MoveToPoint->SetRelativeLocation(FVector(560.0f,  0.0f, -90.0f));

}
