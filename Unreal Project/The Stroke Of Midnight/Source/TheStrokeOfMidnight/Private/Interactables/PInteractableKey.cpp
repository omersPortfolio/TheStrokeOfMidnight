//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar


#include "Interactables/PInteractableKey.h"
#include "Characters/PPlayer.h"

void APInteractableKey::Interact()
{
	// Adds the Key Channel to the Player.
	Character->AddKey(KeyChannel);

	// Destroy self.
	Destroy();
}
