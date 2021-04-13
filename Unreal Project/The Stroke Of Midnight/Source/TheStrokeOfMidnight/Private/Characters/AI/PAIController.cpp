//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar


#include "Characters/AI/PAIController.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "Perception/AISenseConfig_Sight.h"
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISenseConfig_Hearing.h>
#include "Characters/AI/INT_AIHelper.h"
#include <NavigationSystem.h>


APAIController::APAIController()
{
	SetupPerception();

	// Set HearingQueryExtent vector.
	HearingQueryExtent = FVector(75.0f, 75.0f, 150.0f);

	// Set the keys.
	StateKey = FName("State");
	MoveToKey = FName("MoveToLocation");
	PointOfInterestKey = FName("PointOfInterest");
	TargetActorKey = FName("TargetActor");

	// Add "Player" to NoiseTriggerKeys array.
	NoiseTriggerKeys.Add(FName("Player"));
}

void APAIController::SetupPerception()
{
	// Create and initialize sight configuration
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig->SightRadius = 1600;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 400.0f;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;
	SightConfig->SetMaxAge(1.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Create and initialize hearing configuration
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	HearingConfig->HearingRange = 1000;
	HearingConfig->SetMaxAge(1.0f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Set Sight as the dominant sense.
	PerceptionComp->SetDominantSense(*SightConfig->GetSenseImplementation());
	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &APAIController::OnUpdated);
}

void APAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunBehaviorTree(BehaviorTree);
}

void APAIController::OnUpdated(const TArray<AActor*>& UpdatedActors)
{
	FActorPerceptionBlueprintInfo PerceptionInfo;
	// Loop through UpdatedActors
	for (AActor* Actor : UpdatedActors)
	{
		GetAIPerceptionComponent()->GetActorsPerception(Actor, PerceptionInfo);
		SensedActor = PerceptionInfo.Target;
		for (FAIStimulus Stimulus : PerceptionInfo.LastSensedStimuli)
		{
			// Set ActorSensedStimuli to LastSensedStimuli
			ActorSensedStimuli = Stimulus;
			// Is Sight Sense Triggered
			if (UAIPerceptionSystem::GetSenseClassForStimulus(this, ActorSensedStimuli) == UAISense_Sight::StaticClass())
			{
				ProcessVision();
			}
			// Is Hearing Sense Triggered
			else if (UAIPerceptionSystem::GetSenseClassForStimulus(this, ActorSensedStimuli) == UAISense_Hearing::StaticClass())
			{
				ProcessHearing();
			}
		}
	}
}

void APAIController::ProcessVision()
{
	if (SensedActor->GetClass()->ImplementsInterface(UINT_AIHelper::StaticClass()))
	{
		// Seen Player
		if (IINT_AIHelper::Execute_IsActorPlayer(SensedActor))
		{
			// Player is not dead
			if (!IINT_AIHelper::Execute_IsDead(SensedActor))
			{
				if (ActorSensedStimuli.WasSuccessfullySensed())
				{
					// Chase the Sensed Actor.
					SetStateAsAttacking(SensedActor);
				}
				// notSuccessfullySensed
				else 
				{
					// Go to the Stimulus Location.
					SetStateAsSeeking(ActorSensedStimuli.StimulusLocation);
				}
			}
			// IsTargetDead. TODO: THIS IS CURRENTLY NOT CALLED AT ALL. FIX IT
			else 
			{
				// Go back to Patrolling / Idle.
				SetStateAsPassive();
			}
		}
		// Sensed actor is not Player (Might remove later)
		else 
		{
			// if a sensed ally is dead
			if (IINT_AIHelper::Execute_IsDead(SensedActor)) 
			{
				// Aggressively investigate the area 
				SetStateAsSeeking(ActorSensedStimuli.StimulusLocation);
			}
		}
	}
}

void APAIController::ProcessHearing()
{
	FNavLocation NavLocation;
	bool NavResult = UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(ActorSensedStimuli.StimulusLocation, NavLocation, HearingQueryExtent);
	
	if (ActorSensedStimuli.WasSuccessfullySensed() && NavResult)
	{
		// if the heard noise has a Trigger Tag (i.e "Player")
		if (NoiseTriggerKeys.Contains(ActorSensedStimuli.Tag)) 
		{
			// Aggressively investigate the area.
			SetStateAsSeeking(NavLocation);
		}
		// heard noise does not have a Trigger Tag
		else
		{
			// Passively investigate the area.
			SetStateAsInvestigating(NavLocation, ActorSensedStimuli.StimulusLocation);
		}
	}
}

void APAIController::SetState(EAIState State, FVector moveToLocation, FVector pointOfInterest, FVector searchLocation, AActor* targetActor)
{
	// Call the respective functions based on AIState.
	switch (State)
	{
	case Passive:
		SetStateAsPassive();
		break;
	case Investigating:
		SetStateAsInvestigating(moveToLocation, pointOfInterest);
		break;
	case Seeking:
		SetStateAsSeeking(searchLocation);
		break;
	case Attacking:
		SetStateAsAttacking(targetActor);
		break;
	default:
		break;
	}
}

void APAIController::SetStateAsPassive()
{
	// Send the message to the controlled pawn that the State has been set to Passive.
	IINT_AIHelper::Execute_NotifyStateChanged(GetPawn(), EAIState::Passive);

	// Set bSuccessfullySensed to false.
	if (ActorSensedStimuli.IsValid())
	{
		ActorSensedStimuli.MarkNoLongerSensed();
	}

	// Clear all Keys
	GetBlackboardComponent()->ClearValue(MoveToKey);
	GetBlackboardComponent()->ClearValue(PointOfInterestKey);
	GetBlackboardComponent()->ClearValue(TargetActorKey);

	// Set the StateKey to EAIState::Passive
	GetBlackboardComponent()->SetValueAsEnum(StateKey, EAIState::Passive);
}

void APAIController::SetStateAsInvestigating(FVector MoveToLocation, FVector PointOfInterest)
{
	// Send the message to the controlled pawn that the State has been set to Investigating.
	IINT_AIHelper::Execute_NotifyStateChanged(GetPawn(), EAIState::Investigating);
	// Set MoveToKey to MoveToLocation
	GetBlackboardComponent()->SetValueAsVector(MoveToKey, MoveToLocation);
	// Set PointOfInterestKey to PointOfInterest
	GetBlackboardComponent()->SetValueAsVector(PointOfInterestKey, PointOfInterest);
	// Set StateKey to EAIState::Investigating
	GetBlackboardComponent()->SetValueAsEnum(StateKey, EAIState::Investigating);
}

void APAIController::SetStateAsSeeking(FVector SearchLocation)
{
	// Send the message to the controlled pawn that the State has been set to Seeking.
	IINT_AIHelper::Execute_NotifyStateChanged(GetPawn(), EAIState::Seeking);
	// Set StateKey to EAIState::Seeking
	GetBlackboardComponent()->SetValueAsEnum(StateKey, EAIState::Seeking);
	// Set StateKey to EAIState::Investigating
	GetBlackboardComponent()->SetValueAsVector(MoveToKey, SearchLocation);
}

void APAIController::SetStateAsAttacking(AActor* TargetActor)
{
	// Send the message to the controlled pawn that the State has been set to Attacking.
	IINT_AIHelper::Execute_NotifyStateChanged(GetPawn(), EAIState::Attacking);
	// Set StateKey to EAIState::Attacking
	GetBlackboardComponent()->SetValueAsEnum(StateKey, EAIState::Attacking);
	// Set TargetActorKey to TargetActor
	GetBlackboardComponent()->SetValueAsObject(TargetActorKey, TargetActor);
}