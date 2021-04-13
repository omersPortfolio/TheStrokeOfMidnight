//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar


#include "Interactables/PInteractableAudioLog.h"
#include "Components/AudioComponent.h"

APInteractableAudioLog::APInteractableAudioLog()
{
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComp->SetupAttachment(RootComp);
	AudioComp->bAutoActivate = false;

	
	bInterruptible = false;
}

void APInteractableAudioLog::Interact()
{
	// If AudioComp has a valid sound.
	if (AudioComp->Sound)
	{
		// If AudioComp is NOT playing
		if (!AudioComp->IsPlaying())
		{
			// Then play the sound.
			AudioComp->Play();
		}
		// If the AudioComp is playing and is Interruptible
		else if (AudioComp->IsPlaying() && bInterruptible)
		{
			// Then stop the sound.
			AudioComp->Stop();
		}
	}
}
