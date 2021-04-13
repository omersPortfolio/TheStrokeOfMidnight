//Copyright 2021 Memento Game Studios. All Rights Reserved.

/*Changelog:

	01/30/21
		- Transfer of files to new repository
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THESTROKEOFMIDNIGHT_API APPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;
	
};
