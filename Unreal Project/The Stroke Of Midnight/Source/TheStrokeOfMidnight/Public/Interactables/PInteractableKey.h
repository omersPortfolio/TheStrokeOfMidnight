//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	01/30/21
		- Transfer of files to new repository
	03/10/21
		- Removed unused debug message

	03/14/21
		- Removed constructor.
		- Added new comments.
*/

#pragma once

#include "CoreMinimal.h"
#include "PInteractableBase.h"
#include "PInteractableKey.generated.h"

/**
 * 
 */
UCLASS()
class THESTROKEOFMIDNIGHT_API APInteractableKey : public APInteractableBase
{
	GENERATED_BODY()

public:
	// Key Channel that this key holds.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	int KeyChannel;

private:
	virtual void Interact() override;
};
