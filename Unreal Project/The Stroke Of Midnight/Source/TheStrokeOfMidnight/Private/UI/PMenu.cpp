// Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar


#include "UI/PMenu.h"
#include <Camera/CameraComponent.h>
#include <Components/WidgetComponent.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerController.h>
#include "UI/PMenuWidget.h"

// Sets default values
APMenu::APMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	RootComponent = RootComp;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComp);

	MenuWidget = CreateDefaultSubobject<UWidgetComponent>("MenuWidget");
	MenuWidget->SetupAttachment(RootComp);
	
	CameraBlendSpeed = 4.0f;
}

void APMenu::BeginPlay()
{
	Super::BeginPlay();

	UPMenuWidget* Widget = Cast<UPMenuWidget>(MenuWidget->GetUserWidgetObject());
	Widget->OnTransitionToMenu.AddDynamic(this, &APMenu::OnTransitionToMenus);
	SetActorEnableCollision(false);
}

void APMenu::TransitionTo(APMenu* NewMenu)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PC->bEnableClickEvents = false;
	SetActorEnableCollision(false);
	
	
	PC->SetViewTargetWithBlend(NewMenu, CameraBlendSpeed, EViewTargetBlendFunction::VTBlend_Cubic, CameraBlendSpeed);
	FTimerDelegate TeleportDelegate = FTimerDelegate::CreateUObject(this, &APMenu::OnTransitionEnds, PC, NewMenu);
	GetWorldTimerManager().SetTimer(TimerHandle, TeleportDelegate, CameraBlendSpeed - 0.25f, false);
}

void APMenu::OnTransitionEnds(APlayerController* PC, APMenu* NewMenu)
{
	PC->bEnableClickEvents = true;
	NewMenu->SetActorEnableCollision(true);
}

void APMenu::OnTransitionToMenus(int MenuIndex)
{
	if (LinkedMenus.IsValidIndex(MenuIndex))
	{
		TransitionTo(LinkedMenus[MenuIndex]);
	}
}

