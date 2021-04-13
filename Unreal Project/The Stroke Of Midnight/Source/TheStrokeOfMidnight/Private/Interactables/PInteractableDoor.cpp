//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar


#include "Interactables/PInteractableDoor.h"
#include <Kismet/KismetMathLibrary.h>
#include "Characters/PPlayer.h"
#include "Framework/PGameInstance.h"
#include "SaveLoad/PSaveGame.h"

APInteractableDoor::APInteractableDoor()
{
	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>("FrameMesh");
	FrameMesh->SetupAttachment(RootComp);

	Mesh->SetupAttachment(FrameMesh);

	bOpen = false;
	bReadyState = true;

	bRequiresKey = false;
}

void APInteractableDoor::BeginPlay()
{
	Super::BeginPlay();

	RotateValue = -1.0f;

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("ControlDoor"));
		TimelineFinishedCallback.BindUFunction(this, FName("SetStateToTrue"));

		MyTimeline.AddInterpFloat(CurveFloat, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

void APInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);
}

void APInteractableDoor::Interact()
{
	// if the door doesn't require a key OR it requires a key and the player has the same KeyChannel as this door.
	if (!bRequiresKey || (bRequiresKey && Character->HasKey(KeyChannel)))
	{
		// if the door is ready.
		if (bReadyState)
		{
			// reverse the bOpen State
			bOpen = !bOpen;

			// Get the player's location.
			FVector PlayerLocation = Character->GetActorLocation();

			// Get the direction from the player towards this door.
			FVector Direction = GetActorLocation() - PlayerLocation;

			// Set Direction to the inverse of this door's rotation.
			Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, GetActorRotation());

			// Set DoorRotation to Mesh's Relative rotation.
			DoorRotation = Mesh->GetRelativeRotation();

			// if the door is opening
			if (bOpen)
			{
				// if the Direction's X value is greater than 0
				if (Direction.X > 0.0f)
				{
					// Set the rotate value to 1
					RotateValue = 1.0f;
				}
				// if the Direction's X value is less than 0
				else
				{
					// Set the rotate value to -1
					RotateValue = -1.0f;
				}

				// Set the door to be unready.
				bReadyState = false;
				// Play the Timeline from start.
				MyTimeline.PlayFromStart();
			}
			// if the door is closing.
			else
			{
				// set the door to be unready.
				bReadyState = false;
				// Play the timeline from the end.
				MyTimeline.ReverseFromEnd();
			}
		}
	}
}

void APInteractableDoor::SetStateToTrue()
{
	// Set bReadyState to true. From here, the door is now ready to be opened / closed again.
	bReadyState = true;
}

void APInteractableDoor::ControlDoor()
{
	// Get the timeline's playback position
	TimelinePosition = MyTimeline.GetPlaybackPosition();

	// Get the CurveFloatValue
	CurveFloatValue = RotateValue * CurveFloat->GetFloatValue(TimelinePosition);

	// Get the rotation to rotate the door towards.
	FQuat NewRotation = FQuat(FRotator(0.0f, CurveFloatValue, 0.0f));

	// Set the door's rotation to NewRotation
	Mesh->SetRelativeRotation(NewRotation);
}

