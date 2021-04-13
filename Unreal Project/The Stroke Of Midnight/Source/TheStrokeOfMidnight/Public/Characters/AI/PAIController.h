//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	02/02/21
		- AI Controller created
	02/09/21
		- PeripheralVisionAngleDegrees changed from 90.0f to 150.0f
	02/13/21
		- Controller recreated and overhauled for new version of Maynard
		- Behaviour tree functionality added
	02/14/21
		- SpawnLocation Key added
		- Adjustments to accomodate for enum renaming
	02/16/21
		- Forward declarations moved to the top of the file
	02/26/21
		- ProcessLastVisionStimuli renamed to ProcessVision
		- ProcessLastHearingStimuli renamed to ProcessHearing
		- Various comments added and/or rephrased
		- Perception functions and State setting function rearranged
	03/04/21
		- ProjectPointToNavigation FVector parameter changed from 50, 50, 200 to 75, 75, 150
		- Key clearing added to Passive state
	03/05/21
		- Set age of senses from 5 seconds to 1
		- Removed SpawnLocation key
	03/10/21
		- HearingQueryExtent FVector added
		- Removed the Dead state setter
	03/15/21
		- Added new comments

*/

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AIPerceptionTypes.h>
#include "INT_AIHelper.h"
#include "PAIController.generated.h"

class UBehaviorTree;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAIPerceptionComponent;
class AActor;

// AIMaynardController
UCLASS()
class THESTROKEOFMIDNIGHT_API APAIController : public AAIController, public IINT_AIHelper
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	AActor* SensedActor;

	// Sight or Hearing stimuli that's sensed
	UPROPERTY(BlueprintReadOnly)
	FAIStimulus ActorSensedStimuli;

	// Noise Tags that will trigger Maynard's Seeking State
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	TArray<FName> NoiseTriggerKeys;

	// How far can Maynard hear
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	FVector HearingQueryExtent;

	// Keys
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blackboard Keys")
	FName StateKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blackboard Keys")
	FName MoveToKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blackboard Keys")
	FName PointOfInterestKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blackboard Keys")
	FName TargetActorKey;

private:
	UAISenseConfig_Sight* SightConfig;
	UAISenseConfig_Hearing* HearingConfig;

public:
	APAIController();

	// Main State function that's called from the AI Tasks
	UFUNCTION(BlueprintCallable)
	void SetState(EAIState State, FVector moveToLocation, FVector pointOfInterest, FVector searchLocation, AActor* targetActor);

	UFUNCTION()
	void OnUpdated(const TArray<AActor*>& UpdatedActors);
	
private:
	void SetupPerception();

	void ProcessVision();
	void ProcessHearing();

	// Specific State functions
	void SetStateAsPassive();
	void SetStateAsInvestigating(FVector MoveToLocation, FVector PointOfInterest);
	void SetStateAsSeeking(FVector SearchLocation);
	void SetStateAsAttacking(AActor* TargetActor);

	void OnPossess(APawn* InPawn) override;
};
