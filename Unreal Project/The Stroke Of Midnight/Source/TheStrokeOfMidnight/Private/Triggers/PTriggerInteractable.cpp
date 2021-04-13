//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar


#include "Triggers/PTriggerInteractable.h"
#include "Interactables/PInteractableBase.h"
#include "Interactables/PInteractableDoor.h"

void APTriggerInteractable::Event(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (!bTriggered)
	{
		// if the overlapped actor is Player.
		if (OtherActor == Character && OtherActor != this)
		{
			for (AActor* Actor : Actors)
			{
				// if an actor is selected and is a child of APInteractableBase.
				if (Actor && Actor == Cast<APInteractableBase>(Actor))
				{
					// Get the Interactable Actor.
					APInteractableBase* Interactable = Cast<APInteractableBase>(Actor);
					// Call the Interact function of the Interactable actor.
					Interactable->Interact();
				}
			}

			Super::Event(OverlappedActor, OtherActor);
		}
	}
}

#if WITH_EDITOR
void APTriggerInteractable::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Force set bSpawnOnTriggerEnd to false in editor.
	bSpawnOnTriggerEnd = false;
}
#endif