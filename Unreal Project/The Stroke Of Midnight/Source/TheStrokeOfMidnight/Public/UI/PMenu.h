// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PMenu.generated.h"

class UCameraComponent;
class UWidgetComponent;

UCLASS()
class THESTROKEOFMIDNIGHT_API APMenu : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APMenu();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* MenuWidget;

	UFUNCTION(BlueprintCallable)
	void TransitionTo(APMenu* NewMenu);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3DTransition")
	TArray<APMenu*> LinkedMenus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3DTransition")
	float CameraBlendSpeed;

	FTimerHandle TimerHandle;

	void OnTransitionEnds(APlayerController* PC, APMenu* NewMenu);

	UFUNCTION()
	void OnTransitionToMenus(int MenuIndex);

protected:
	virtual void BeginPlay() override;

};
