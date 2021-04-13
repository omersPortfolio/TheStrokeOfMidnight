//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Zoe Purcell

/*Changelog:

	02/21/21
		- Statue AI created
		- Sight perception removed for a simple activation distance
		- Player Key added
		- Large amount of functionality removed from C++ and properly implemented in behaviour tree
	03/04/21
		- Forward declarations moved to top of file
	03/10/21
		- Moved Key naming from constructor to OnPossess
		- Added a null check before setting the Player as as the PlayerKey
	02/28/21
		- Added back sight component
		- Statue now uses AI States
		- Statue will now return to dormant state when player is out of line of sight, and activate again when they see the player
*/

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"

#include "PStatueController.generated.h"

class UAISenseConfig_Sight;
class APPlayer;
/**
 * 
 */
UCLASS()
class THESTROKEOFMIDNIGHT_API APStatueController : public AAIController
{
	GENERATED_BODY()

public:
	APStatueController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	UBehaviorTree* BehaviorTree;
	
	// Player Key for Blackboard to reference from.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blackboard Keys")
	FName PlayerKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blackboard Keys")
	FName StateKey;

private:
	AActor* SensedActor;

	UAISenseConfig_Sight* SightConfig;

	FAIStimulus ActorSensedStimuli;

private:
	virtual void BeginPlay() override;
	
	void ProcessLastVisionStimuli();

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
};
