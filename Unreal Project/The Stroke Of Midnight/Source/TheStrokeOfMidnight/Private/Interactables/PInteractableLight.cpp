//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar 


#include "Interactables/PInteractableLight.h"
#include "Components/PointLightComponent.h"

APInteractableLight::APInteractableLight()
{
	Light = CreateDefaultSubobject<UPointLightComponent>("Light");
	Light->SetupAttachment(RootComponent);

	// Variables
	Intensity = 10000.0f;
	LightRadius = 350.0f;
}

void APInteractableLight::BeginPlay()
{
	Super::BeginPlay();

	// Set the intensity to Intensity value or 0 based on State
	if (State)
	{
		Light->SetIntensity(Intensity);
	}
	else
	{
		Light->SetIntensity(0.0f);
	}

	// Set the Light's radius to be LightRadius
	Light->SetAttenuationRadius(LightRadius);
}

void APInteractableLight::Interact()
{
	// if State is on
	if (State)
	{
		// Turn off the light.
		Light->SetIntensity(0);
	}
	// if State is off
	else
	{
		// Turn on the light.
		Light->SetIntensity(Intensity);
	}
	// Reverse the state.
	State = !State;

}
