//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Zoe Purcell


#include "Characters/AI/PStatueController.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "Characters/PPlayer.h"
#include "Perception/AISenseConfig_Sight.h"
#include <Perception/AIPerceptionComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Characters/AI/INT_AIHelper.h"

APStatueController::APStatueController()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->SightRadius = 5000;
	SightConfig->PeripheralVisionAngleDegrees = 360.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComp->SetDominantSense(*SightConfig->GetSenseImplementation());
	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &APStatueController::OnPerceptionUpdated);
	PerceptionComp->ConfigureSense(*SightConfig);

	PlayerKey = FName("TargetActor");
	StateKey = FName("State");
}

void APStatueController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
}
void APStatueController::ProcessLastVisionStimuli()
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
					GetBlackboardComponent()->SetValueAsEnum(StateKey, EAIState::Attacking);
					GetBlackboardComponent()->SetValueAsObject(PlayerKey, SensedActor);
				}
				// notSuccessfullySensed
				else
				{
					GetBlackboardComponent()->SetValueAsEnum(StateKey, EAIState::Passive);
				}
			}
		}
	}
}

void APStatueController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	FActorPerceptionBlueprintInfo PerceptionInfo;
	for (AActor* Actor : UpdatedActors)
	{
		GetAIPerceptionComponent()->GetActorsPerception(Actor, PerceptionInfo);
		SensedActor = PerceptionInfo.Target;

		for (FAIStimulus Stimulus : PerceptionInfo.LastSensedStimuli)
		{
			ActorSensedStimuli = Stimulus;
			if (Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
			{
				ProcessLastVisionStimuli();
			}
		}
	}
}


