//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	01/31/21
		- Interactable Audio Logs created.
	03/10/21
		- Renamed bPlayOnce to bInterruptable
		- Added functionality to stop an audio recording before it's finished playing
	03/16/21
		- Added new comments.
		- Moved forwards declaration to the top.

*/

#pragma once

#include "CoreMinimal.h"
#include "PInteractableBase.h"
#include "PInteractableAudioLog.generated.h"

class UAudioComponent;

UCLASS()
class THESTROKEOFMIDNIGHT_API APInteractableAudioLog : public APInteractableBase
{
	GENERATED_BODY()

public:
	APInteractableAudioLog();

	virtual void Interact() override;

	// Audio Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComp;

	// Whether the Audio Log can be stopped while playing.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bInterruptible;
};
