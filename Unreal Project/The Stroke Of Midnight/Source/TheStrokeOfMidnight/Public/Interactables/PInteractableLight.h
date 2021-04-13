//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	01/30/21
		- Transfer of files to new repository

	03/14/21
		- Added new comments.
	03/16/21 
		- Added a new variable named LightRadius for custom Attenuation Radius.
		- Added new comments.

*/

#pragma once

#include "CoreMinimal.h"
#include "PInteractableBase.h"
#include "PInteractableLight.generated.h"

class UPointLightComponent;

UCLASS()
class THESTROKEOFMIDNIGHT_API APInteractableLight : public APInteractableBase
{
	GENERATED_BODY()

public:
	
	APInteractableLight();

	// PointLightComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPointLightComponent* Light;

	
	// Light Intensity value
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "0"))
	float Intensity;

	// Light Attenuation Radius
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "0"))
	float LightRadius;

	bool GetState() const { return State; }
	void SetState(bool newState) { State = newState; }

private:
	// Virtual Interact function
	virtual void Interact() override;

	// On/Off state
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess))
	bool State;

private:
	virtual void BeginPlay() override;
};
