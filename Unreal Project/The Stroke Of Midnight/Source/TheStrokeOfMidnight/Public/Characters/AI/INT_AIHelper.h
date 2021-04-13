//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	02/13/21
		- Interface moved from Blueprints to C++
		- ModifyMovementSpeed renamed to UpdateMovementSpeed
		- GetPatrolPath function created
	02/15/21
		- Attack function added
		- GetAttackRange function added
	02/16/21
		- MatchActorSpeed function added
	02/26/21
		- Comments added

*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "INT_AIHelper.generated.h"

class APPatrolPath;
class USceneComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UINT_AIHelper : public UInterface
{
	GENERATED_BODY()
};

class THESTROKEOFMIDNIGHT_API IINT_AIHelper
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIHelper")
	bool IsDead();

	// Notifies a target (i.e AnimBP) that our State has changed.
	// Example: When an AI enters Death State, it notifies the AI's AnimBP and the AI plays the death Animation.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIHelper")
	void NotifyStateChanged(EAIState NewState);

	// Among the actors that has this Interface, is it Player?
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIHelper")
	bool IsActorPlayer();

	// Updates movement speed based on AI State
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIHelper")
	void UpdateMovementSpeed(EAIState State);

	// Matches the target's speed with an offset while chasing.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIHelper")
	void MatchActorSpeed(AActor* ActorToMatchSpeed, float OffsetSpeed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIHelper")
	void SetMovementSpeed(float NewSpeed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIHelper")
	APPatrolPath* GetPatrolPath();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIHelper")
	void Attack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIHelper")
	float GetAttackRange();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIHelper")
	USceneComponent* GetMoveToPoint();

};
