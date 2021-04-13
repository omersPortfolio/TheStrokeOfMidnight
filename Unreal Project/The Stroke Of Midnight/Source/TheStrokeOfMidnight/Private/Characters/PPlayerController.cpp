//Copyright 2021 Memento Game Studios. All Rights Reserved.


#include "Characters/PPlayerController.h"

void APPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	
	bShowMouseCursor = false;
}
