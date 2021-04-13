//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

#include "Triggers/PTriggerSequence.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequence/Public/LevelSequenceActor.h"

void APTriggerSequence::Event(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (!bTriggered)
	{
		if (OtherActor == Character && OtherActor != this && LevelSequence)
		{
			LevelSequence->GetSequencePlayer()->Play();

			Super::Event(OverlappedActor, OtherActor);
		}
	}	
}

#if WITH_EDITOR
void APTriggerSequence::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Force set bActorSelect to true in editor.
	bActorSelect = false;
	
	// Force set bSpawnOnTriggerEnd to false in editor.
	bSpawnOnTriggerEnd = false;
}
#endif