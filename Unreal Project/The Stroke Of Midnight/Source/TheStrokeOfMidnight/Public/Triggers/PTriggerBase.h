//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	01/30/21
		- Transfer of files to new repository
	03/04/21
		- Forward declarations moved to top of file
	03/10/21
		- Actor spawning moved from TriggerLookAt to here
	03/11/21
		- Noise making moved from the now-deleted TriggerNoise to here
		- Various comment edits
		- Various UProperty tweaks
	03/14/21
		- Added new comments.
		- Changed bDestroySelfAfterTrigger to bDestroySelfWithDelay
	03/15/21
		- Added PostEditChangeProperty function.
		- Made UPROPERTY meta changes.
	03/22/21
		- Removed the destruction feature from Triggers and replaced it with a boolean (bTriggered) that disables trigger.
	03/23/21
		- Created GetActors function
	03/31/21
		- Added Particle Emitter functionality on Spawned Actor Destroy.
	04/02/21
		- Added bTriggerOnSight variable to make it easier to set up Sight Trigger.
	04/05/21
		- Added Camera Shake feature on Trigger.
	08/04/21
		- Added GUID for save and load
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Characters/PPlayer.h"
#include "PTriggerBase.generated.h"

class AActor;
class USoundBase;
class UTextRenderComponent;
class APPlayer;
class UPGameInstance;

UCLASS()
class THESTROKEOFMIDNIGHT_API APTriggerBase : public ATriggerBox
{
	GENERATED_BODY()

public:
	APTriggerBase();

	UFUNCTION()
	virtual void Event(AActor* OverlappedActor, AActor* OtherActor); //PURE_VIRTUAL(APTriggerBase::Event, );

	// Spawn Logic
	void Spawn();

	bool bTriggered = false;

	inline TArray<AActor*> GetActors() { return Actors; }

	inline FGuid GetGuid() { return Guid; }
	
protected:
	virtual void BeginPlay() override;

	void PlayDestroyEmitter(AActor* ActorToSpawn);

	void PlayDestroySound(AActor* ActorToSpawn);

	FGuid Guid;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

	// Whether the player can trigger this by looking at it from a distance.
	UPROPERTY(EditAnywhere, Category = "Trigger")
	bool bTriggerOnSight = false;
	
	// Whether to select actor.
	UPROPERTY(EditAnywhere, Category = "Trigger|ActorSelection")
	bool bActorSelect;

	// Actors to manipulate.
	UPROPERTY(EditAnywhere, Category = "Trigger|ActorSelection", meta = (EditCondition = bActorSelect, EditConditionHides))
	TArray<AActor*> Actors;

	// Sound

	UPROPERTY(EditAnywhere, Category = "Trigger|Sound")
	USoundBase* TriggerSound; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTextRenderComponent* Text;

	UPROPERTY(EditAnywhere, Category = "Trigger|Sound", meta = (UIMin = "0", UIMax = "10", EditCondition = "TriggerSound != nullptr", EditConditionHides))
	float VolumeMultiplier = 1.0f;

	/////////////// Spawn //////////////////////
	////////////// an Object ///////////////////

	// Whether to spawn an object.
	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn")
	bool bWillSpawn = false;

	// False = Spawns right away.
	// True = Spawns After Trigger Ends.
	// Currently only applicable to LookAt. OTHERWISE DO NOT SET TO TRUE OR IT WON'T SPAWN! 
	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn|TriggerLookAtExclusive", meta = (EditCondition = bWillSpawn, EditConditionHides))
	bool bSpawnOnTriggerEnd = false;

	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = bWillSpawn, EditConditionHides))
	TSubclassOf<AActor> ClassToSpawn;

	// Whether to destroy the spawned actor.
	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = bWillSpawn, EditConditionHides, ClampMin = "0.1"))
	bool bDestroySpawnedActor;

	// Life Span of the spawned actor.
	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = "bDestroySpawnedActor", EditConditionHides, ClampMin = "0.1"))
	float SpawnedActorLifeSpan = 0.1f;

	/*True = Spawn at World Location.         
False = Spawn at the location of the selected Actor.*/
	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = bWillSpawn, EditConditionHides))
	bool bLocationSelect;

	// World Location	
	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = "bLocationSelect", EditConditionHides))
	FVector SpawnLocation;

	// Location of the selected actor.
	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = "!bLocationSelect && bWillSpawn", EditConditionHides))
	AActor* ActorLocationToSpawn;

	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = bWillSpawn, EditConditionHides))
	FRotator SpawnRotation;

	// Whether to spawn a particle emitter on Spawned Actor Destroy.
	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = bWillSpawn, EditConditionHides))
	bool bSpawnParticle;

	// Particle to spawn on Spawned Actor Destroy.
	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = "bWillSpawn && bSpawnParticle", EditConditionHides))
	UParticleSystem* DestroyParticle;

	// How early should the Particle spawn. Default to 0.085. 
//Do not change it if you want to spawn it as the actor is being destroyed.
	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = "bWillSpawn && bSpawnParticle", EditConditionHides, ClampMin = "0.0"))
	float ParticlePreSpawnTime;

	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = bWillSpawn, EditConditionHides))
	bool bPlaySoundOnDestroy;

	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = "bWillSpawn && bPlaySoundOnDestroy", EditConditionHides))
	USoundBase* DestroySound;

	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = "bWillSpawn && bPlaySoundOnDestroy", EditConditionHides, DisplayName = "VolumeMultiplier", UIMin = "0.0"))
	float DestroySoundVolumeMultiplier;

	// How early should the Sound play. Default to 0.085. 
//Do not change it if you want to play it as the actor is being destroyed.
	UPROPERTY(EditAnywhere, Category = "Trigger|Spawn", meta = (EditCondition = "bWillSpawn && bPlaySoundOnDestroy", EditConditionHides, ClampMin = "0.0"))
	float SoundPrePlayTime;

	////////// CameraShake //////////////

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|CameraShake")
	bool bPlayCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|CameraShake", meta = (EditCondition = bPlayCameraShake, EditConditionHides))
	TSubclassOf<UCameraShake> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|CameraShake", meta = (EditCondition = bPlayCameraShake, EditConditionHides))
	float CameraShakeScale = 1.0f;

	///////////////		 //////////////////////
	//			   Noise					///
	//////////////       //////////////////////

	// Whether to make noise on Trigger
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Noise")
	bool bMakesNoise;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Noise", meta = (EditCondition = "bMakesNoise", EditConditionHides))
	float NoiseLoudness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Noise", meta = (EditCondition = "bMakesNoise", EditConditionHides))
	float NoiseRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Noise", meta = (EditCondition = "bMakesNoise", EditConditionHides))
	FName NoiseTag;

protected:
	// Find a way to get rid of this. Maybe another variable for Sound Location.
	AActor* SoundActor;
	APPlayer* Character;

	FTimerHandle ParticleTimerHandle;
	FTimerHandle DestroySoundTimerHandle;
};
