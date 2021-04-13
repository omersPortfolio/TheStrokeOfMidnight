//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar and Zoe Purcell


#include "Characters/AI/PEnemyBase.h"
#include "Characters/PPlayer.h"
#include "Kismet/GameplayStatics.h"

void APEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}
