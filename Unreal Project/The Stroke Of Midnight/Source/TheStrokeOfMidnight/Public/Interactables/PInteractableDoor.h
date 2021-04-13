//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	01/30/21
		- Transfer of files to new repository

	03/14/21
		- Added new comments
	03/16/21
		- Added new comments
*/

#pragma once

#include "CoreMinimal.h"
#include "PInteractableBase.h"
#include "Components/TimelineComponent.h"
#include "PInteractableDoor.generated.h"

// TODO: Might make a different class for the Door that requires key due to some bugs. 


UCLASS()
class THESTROKEOFMIDNIGHT_API APInteractableDoor : public APInteractableBase
{
	GENERATED_BODY()

public:
	APInteractableDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FrameMesh;

	// Curve float for door's opening/closing.
	UPROPERTY(EditAnywhere, Category = "Interaction")
	UCurveFloat* CurveFloat;

	// Whether the door requires a key
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bRequiresKey;

	/*KeyChannel that has to match with the channel of the key that the player picks up.
	This is only activated in BP if bRequiresKey is true.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (EditCondition = "bRequiresKey", EditConditionHides))
	int KeyChannel;

	inline bool GetOpen() { return bOpen; }
	inline void SetOpen(bool open) { bOpen = open; }
	inline FTimeline GetTimeline() { return MyTimeline; }

private:
	// Timeline for Door's rotation
	FTimeline MyTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bOpen;

	// is the Door Ready to Open/Close
	bool bReadyState;

	// Rotation value to multiply with Door's open/close rotation. is either 1 or -1.
	float RotateValue;

	// the value inside the CurveFloat.
	float CurveFloatValue;

	// Timeline's position at a given time
	float TimelinePosition;

	FRotator DoorRotation;	

public:
	// Handles the rotation of the door
	UFUNCTION()
	void ControlDoor();

private:
	virtual void Interact() override;

private:
	// Sets the Door's Ready state to true. Gets called once the timeline ends. 
	UFUNCTION()
	void SetStateToTrue();

private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
