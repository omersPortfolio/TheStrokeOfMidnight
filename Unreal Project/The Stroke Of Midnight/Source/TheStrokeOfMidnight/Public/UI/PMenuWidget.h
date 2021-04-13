// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTransitionToMenu, int, MenuIndex);


UCLASS(Abstract)
class THESTROKEOFMIDNIGHT_API UPMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Widget")
	FTransitionToMenu OnTransitionToMenu;
	
};
