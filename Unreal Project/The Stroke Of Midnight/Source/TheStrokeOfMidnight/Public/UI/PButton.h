// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PButton.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseClicked);

UCLASS()
class THESTROKEOFMIDNIGHT_API UPButton : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Widget")
	FOnMouseClicked OnMouseClicked;
};
