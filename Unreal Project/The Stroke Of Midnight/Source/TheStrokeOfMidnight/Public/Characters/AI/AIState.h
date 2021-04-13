//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	02/13/21
		- States moved form Blueprints to C++
	02/14/21
		- Changed name from AIState to EAIState
	02/26/21
		- Comments added to each state

*/

#pragma once

#include <UObject/ObjectMacros.h>

UENUM(BlueprintType)
enum EAIState
{
	Passive				UMETA(DisplayName = "Passive"), // Idle, Patrolling
	Investigating		UMETA(DisplayName = "Investigating"), // Non-aggressive Investigation
	Seeking				UMETA(DisplayName = "Seeking"), // Aggressive Investigation
	Attacking			UMETA(DisplayName = "Attacking"),
};