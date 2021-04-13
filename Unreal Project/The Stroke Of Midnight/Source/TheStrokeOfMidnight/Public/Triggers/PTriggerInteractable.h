//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	01/30/21
		- Transfer of files to new repository
	03/04/21
		- Removed redundant forward declaration
	03/14/21
		- Added new comments.
	03/15/21
		- Added PostEditChangeProperty function.

*/

#pragma once

#include "CoreMinimal.h"
#include "PTriggerBase.h"
#include "PTriggerInteractable.generated.h"


UCLASS()
class THESTROKEOFMIDNIGHT_API APTriggerInteractable : public APTriggerBase
{
	GENERATED_BODY()

private:
	void Event(AActor* OverlappedActor, AActor* OtherActor) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
