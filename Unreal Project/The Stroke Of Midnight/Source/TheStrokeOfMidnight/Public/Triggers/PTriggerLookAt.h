//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	01/30/21
		- Transfer of files to new repository
	03/04/21
		- Removed redundant forward declaration
	03/10/21
		- Moved Actor spawning from here to TriggerBase
	03/14/21
		- Added new comments.
	03/16/21
		- Changed InterpReturn's name to RotatePlayerCam to make it make more sense to reviewers.

*/

#pragma once

#include "CoreMinimal.h"
#include "PTriggerBase.h"
#include "Components/TimelineComponent.h"
#include "PTriggerLookAt.generated.h"

class APPlayerController;


UCLASS()
class THESTROKEOFMIDNIGHT_API APTriggerLookAt : public APTriggerBase
{
	GENERATED_BODY()

private:
	APTriggerLookAt();

	virtual void BeginPlay() override;

	// This is to clamp InputDisableDuration between 0 and DestroyDelayTime - 0.1f to prevent infinite input disable.
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void Event(AActor* OverlappedActor, AActor* OtherActor) override;

	// Function that's called when Timeline finishes.
	UFUNCTION()
	void OnFinish();

	// Function that has LookAt logic.
	UFUNCTION()
	void RotatePlayerCam(float Value);
    
	// Which object to rotate the camera towards.
	UPROPERTY(EditInstanceOnly, Category = "Trigger")
	AActor* ActorToLookAt;

	/* How long should the player not have input control.
Will always be lower than DestroyDelayTime. */
	UPROPERTY(EditAnywhere, Category = "Trigger")
	float InputDisableDuration;

	// Function that enables input.
	UFUNCTION()
	void InputEnable();

	// Timeline Component
	UPROPERTY()
	UTimelineComponent* LookAtTimeline;

	// Curve Float to set the duration of the camera rotation.
	UPROPERTY(EditAnywhere, Category = "Trigger")
	UCurveFloat* CurveFloat;


private:
	// Timeline Events
	FOnTimelineFloat InterpFunction{};
	FOnTimelineEvent TimelineFinish{};

	bool bInputEnabled = true;

	FTimerHandle TimerHandle;

	// PlayerController
	APPlayerController* PC;
};