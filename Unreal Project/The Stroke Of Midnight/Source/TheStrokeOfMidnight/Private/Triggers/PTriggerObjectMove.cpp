//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar


#include "Triggers/PTriggerObjectMove.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>

APTriggerObjectMove::APTriggerObjectMove()
{
	// Constructor
	bActorSelect = false;
	bDestroyMovedObjects = false;
	ForceMultiply = 1.0f;
}

void APTriggerObjectMove::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* Actor : Actors)
	{
		ForcedActorTransforms.Add(Actor->GetTransform());
	}
}

void APTriggerObjectMove::Event(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (!bTriggered)
	{
		// Is the Overlapped actor Player
		if (OtherActor == Character)
		{
			// Loop through the selected actors to add force to
			for (AActor* Actor : Actors)
			{
				if (Actor)
				{
					// Get the Mesh of the found actor
					UMeshComponent* MeshComp = Actor->FindComponentByClass<UMeshComponent>();
					if (MeshComp)
					{
						// Set Simulate Physics to true on the actor's mesh.
						MeshComp->SetSimulatePhysics(true);
						if (bForceTowardsVector)
						{
							// Add impulse to the mesh towards ForceDirection.
							MeshComp->AddImpulse(ForceDirection * (FMath::RandRange(MinRandomMultiply, MaxRandomMultiply) * ForceMultiply), NAME_None, true);
						}
						// Force towards the selected actor
						else
						{
							// if an actor is selected instead of ForceDirection.
							if (ForceTowardsActor)
							{
								// Get the direction towards ForceTowardsActor.
								FVector Direction = ForceTowardsActor->GetActorLocation() - Actor->GetActorLocation();
								// Add impulse to the actors towards ForceTowardsActor.
								MeshComp->AddImpulse(Direction * (FMath::RandRange(MinRandomMultiply, MaxRandomMultiply) * ForceMultiply), NAME_None, true);
							}
						}
					}

					// if we want to destroy the moved objects.
					if (bDestroyMovedObjects)
					{
						// Set the life span of the moved objects.
						Actor->SetLifeSpan(MovedObjectDestroyTime);
					}

					Super::Event(OverlappedActor, OtherActor);
				}

			}
		}
	}
}

#if WITH_EDITOR
void APTriggerObjectMove::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Force set bSpawnOnTriggerEnd to false in editor.
	bSpawnOnTriggerEnd = false;
}

#endif
