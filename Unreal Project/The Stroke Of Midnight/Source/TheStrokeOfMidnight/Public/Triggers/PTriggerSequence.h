//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	01/30/21
		- Transfer of files to new repository
	03/04/21
		- Removed redundant forward declaration
	03/15/21
		- Added PostEditChangeProperty function.
	03/23/21
		- Added a null check for the level sequence, so there isn't a crash if you forget to add one in the editor
*/

#pragma once

#include "CoreMinimal.h"
#include "PTriggerBase.h"
#include "PTriggerSequence.generated.h"


UCLASS()
class THESTROKEOFMIDNIGHT_API APTriggerSequence : public APTriggerBase
{
	GENERATED_BODY()

public:
	void Event(AActor* OverlappedActor, AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, Category = "Trigger")
	class ALevelSequenceActor* LevelSequence;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
