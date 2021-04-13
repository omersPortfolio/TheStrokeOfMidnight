//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	01/30/21
		- Transfer of files to new repository
	03/04/21
		- Removed redundant forward declaration
	03/11/21
		- TriggerObjectMove overhauled and cleaned up
		- Added functionality to move objects towards a specific Actor
		- Added editable force strength as well as min/max for Random force
		- Added option for object deletion after a certain time
	03/14/21
		- Added new comments.
		- Clamped MovedObjectDestroyTime's minimum number to 0.1.
	03/15/21
		- Added PostEditChangeProperty function.
	03/16/21
		- Changed MeshComp's type from UStaticMeshComponent to UMeshComponent.
	03/23/21
		- Created AddActorsToList function.

*/

#pragma once

#include "CoreMinimal.h"
#include "PTriggerBase.h"
#include "PTriggerObjectMove.generated.h"


UCLASS()
class THESTROKEOFMIDNIGHT_API APTriggerObjectMove : public APTriggerBase
{
	GENERATED_BODY()

public:

	APTriggerObjectMove();

	// True = Force the actor(s) towards a Vector direction.
	// False = Force the actor(s) towards an actor.
	UPROPERTY(EditAnywhere, Category = "Trigger|Force")
	bool bForceTowardsVector;

	// The direction to force the mesh towards.
	UPROPERTY(EditAnywhere, Category = "Trigger|Force", meta = (EditCondition = "bForceTowardsVector", EditConditionHides))
	FVector ForceDirection;

	// The actor that the forced actors will be forced towards.
	UPROPERTY(EditAnywhere, Category = "Trigger|Force", meta = (EditCondition = "!bForceTowardsVector", EditConditionHides))
	AActor* ForceTowardsActor;

	// The amount of force added. 
	// Set to 1 by default
	UPROPERTY(EditAnywhere, Category = "Trigger|Force", meta = (ClampMin = "0.1"))
	float ForceMultiply;

	// Min random number to multiply force with.
	// Set to 1 by default.
	UPROPERTY(EditAnywhere, Category = "Trigger|Force", meta = (ClampMin = "0.1"))
	float MinRandomMultiply = 1.0f;

	// Max random number to multiply force with. 
	// Set to 1 by default.
	UPROPERTY(EditAnywhere, Category = "Trigger|Force", meta = (ClampMin = "0.1"))
	float MaxRandomMultiply = 1.0f;

	// Whether to destroy the moved objects.
	UPROPERTY(EditAnywhere, Category = "Trigger|Destroy")
	bool bDestroyMovedObjects;

	// Time to destroy the moved objects.
	UPROPERTY(EditAnywhere, Category = "Trigger|Destroy", meta = (EditCondition = bDestroyMovedObjects, EditConditionHides, ClampMin = "0.1"))
	float MovedObjectDestroyTime = 1.0f;

	TArray<FTransform> ForcedActorTransforms;

private:
	void Event(AActor* OverlappedActor, AActor* OtherActor) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void BeginPlay() override;
};
