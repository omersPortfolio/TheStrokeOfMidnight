//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

#include "Triggers/PTriggerLookAt.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/PPlayerController.h"
#include <Kismet/KismetMathLibrary.h>
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APTriggerLookAt::APTriggerLookAt()
{
	// Create Timeline Component
	LookAtTimeline = CreateDefaultSubobject<UTimelineComponent>("Timeline");
	InterpFunction.BindUFunction(this, FName("RotatePlayerCam"));
	TimelineFinish.BindUFunction(this, FName("OnFinish"));

	InputDisableDuration = 1.0f;
}

void APTriggerLookAt::BeginPlay()
{
	Super::BeginPlay();

	// Find Player Controller
	PC = Cast<APPlayerController>(GetWorld()->GetFirstPlayerController());
	
	// Setters needed for CurveFloat.
	if (CurveFloat)
	{
		LookAtTimeline->AddInterpFloat(CurveFloat, InterpFunction, FName("Alpha"));
		LookAtTimeline->SetTimelineFinishedFunc(TimelineFinish);
		LookAtTimeline->SetLooping(false);
	}
}

void APTriggerLookAt::Event(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (!bTriggered)
	{
		// If the overlapped actor is Player.
		if (OtherActor == Character && OtherActor != this)
		{
			// if ActorToLookAt is selected.
			if (ActorToLookAt)
			{
				// Play the rotation timeline.
				LookAtTimeline->Play();

				// Disable Player's Input.
				Character->StopSprint();
				Character->DisableInput(PC);
				bInputEnabled = false;

				// Set Timer for EnableInput function
				GetWorldTimerManager().SetTimer(TimerHandle, this, &APTriggerLookAt::InputEnable, InputDisableDuration, false);

				Super::Event(OverlappedActor, OtherActor);
			}
		}
	}
}

void APTriggerLookAt::OnFinish()
{
	LookAtTimeline->SetPlaybackPosition(0, false);
}

void APTriggerLookAt::RotatePlayerCam(float Value)
{
	// If Player Controller is found and its input is disabled.
	if (PC && !bInputEnabled)
	{
		// Get the PC's control rotation
		FRotator ControlRotation = PC->GetControlRotation();

		// Get Player's Camera Component.
		UCameraComponent* PlayerCam = Character->GetCameraComponent();

		// Get ActorToLookAt's location.
		FVector ActorLoc = ActorToLookAt->GetActorLocation();

		// Calculate the rotation from player's camera towards ActorLoc.
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(PlayerCam->GetComponentLocation(), ActorLoc);

		// Interpolate the Player's camera rotation towards LookAtRot
		FRotator LerpedRotation = FMath::RInterpTo(PlayerCam->GetComponentRotation(), LookAtRot, Value, false);

		// Interpolate PC's control rotation towards LerpedRotation.
		FRotator FinalRotation = FMath::RInterpTo(ControlRotation, LerpedRotation, Value, true);

		// Set PC's control rotation to Final Rotation.
		PC->SetControlRotation(FinalRotation);
	}
}

void APTriggerLookAt::InputEnable()
{
	// Enable Player's Input.
	Character->EnableInput(PC);
	bInputEnabled = true;

	// If bSpawnOnTriggerEnd is true, Spawn an actor after input is enabled.
	if (bSpawnOnTriggerEnd)
	{
		Spawn();
	}
}

#if WITH_EDITOR
void APTriggerLookAt::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Force set bActorSelect to false as it is not needed for this Trigger.
	bActorSelect = false;
}
#endif
