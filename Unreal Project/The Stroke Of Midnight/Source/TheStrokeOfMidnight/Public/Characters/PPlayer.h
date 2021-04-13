//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar and Zoe Purcell

/*Changelog:

	01/30/21
		- Transfer of files to new repository
	02/01/21
		- Commented added above Sprint TimerHandle
	02/02/21
		- Perception Stimulus added to the Player
	02/05/21
		- Health Component added to the Player
		- Sprint TimerHandle removed
		- Edited UProperties for Health Component
	02/06/21
		- MaxStamina value added
		- Stamina now uses FMath::Clamp to keep it within 0 and 100
	02/09/21
		- OnDeath function added
		- Input is disabled when the Player dies
		- Camera saturation disabled when the Player dies
		- PlayerController added as a member
		- UProperties edited for RegenerationDelay and RegeneratonIncrementRate
	02/10/21
		- Fixed the previously broken Lighter
		- Camera saturation values upon death now stored as a member
		- bool IsSprinting function added
		- Added !IsSprinting condition to pull out the Lighter
	02/13/21
		- Interface functions IsDead and IsActorPlayer added
	02/15/21
		- Player class now inherits from a Base Character class
		- Health Component moved to parent class
		- OnDeath moved to parent class and is set as virtual. Player class' OnDeath is now an override
		- Renamed from PCharacter to PPlayer
		- Member initialization moved from constructor to Init
		- Functions reordered and reorganized
		- EStaminaState renamed to ECharacterStaminaState
	02/17/21
		- Stamina renamed to CurrentStamina (to match HealthComponents CurrentHealth)
		- Respawn Location member added
		- Player gains cursor usage when they die so they can select the Respawn menu button
		- SetRespawnLocation function added
		- Respawn function added
		- Player will now teleport to their RespawnLocation when they die and click Respawn
		- Player's RespawnLocation now updates when they overlap with a Checkpoint
	03/04/21
		- SprintNoiseRange float added (Player now makes noise when sprinting)
		- Player calls MakeNoise in Sprint
	03/10/21
		- Updated some comments
	03/15/21
		- Added new comments
		- Passed in the newly created Object trace channel for Interactables.
		- Created SetEnableMouse(bool bState) function.
	03/16/21
		- Edited ExhaustionThreshold so it's set to an actual percentage of the MaxStamina rather than a hard-coded value
		- Added a debug message for when the player is exhausted
	03/18/21
		- Created SaveGame, LoadGame and DeleteSaveGame functions.
	03/27/21
		- Player now checks if velocity is > 0 before draining stamina
		- Added functionality where the player can only sprint if they are moving forward
		

*/

#pragma once

#include "CoreMinimal.h"
#include "PCharacterBase.h"
#include "PPlayer.generated.h"

class APPlayerController;
class USpringArmComponent;
class UCameraComponent;
class UPointLightComponent;
class UAIPerceptionStimuliSourceComponent;
class UPHealthComponent;
class APTriggerBase;
class UParticleSystemComponent;
class UPhysicsConstraintComponent;

// Stamina State Enum
UENUM()
enum class EStaminaState : uint8
{
	Draining,
	Regenerating,
};

UCLASS()
class THESTROKEOFMIDNIGHT_API APPlayer : public APCharacterBase
{
	GENERATED_BODY()

// Components
public:
	APPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;

	inline UCameraComponent* GetCameraComponent() { return Camera; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LighterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPointLightComponent* LighterLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* LighterFlame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UAIPerceptionStimuliSourceComponent* Stimulus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HeldObjectSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* PhysicsConstraint;

private:
	// Initializes components and variables
	void Init();

// Interaction
private:
	// One and only function to interact with objects deriving from PInteractableBase
	void Interact();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float InteractionRange;
	// Key Channels that the player acquires from the keys he collects.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TArray<int> KeyChannels;

public:
	//Adds the KeyChannel of the key that the player picks up.
	void AddKey(int KeyChannel);

	//Whether the player has a specific KeyChannel. Door checks this function.
	bool HasKey(int KeyChannel) const;

	const inline TArray<int> GetKeyChannels() { return KeyChannels; }
	inline void SetKeyChannels(TArray<int> ChannelArray) { KeyChannels = ChannelArray; }

	UPROPERTY(BlueprintReadOnly)
	bool bTracedInteractable;

// Walk / Sprint
public:
	void StopSprint();

private:
	void Sprint();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;

	// whether the player can sprint.
	bool bCanSprint;

	// whether the player is sprinting.
	bool IsSprinting();

	// How wide Sprinting should make noise.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float SprintNoiseRange;

// Stamina
private:
	// Stamina State: Draining and Regenerating
	EStaminaState StaminaState;

	// Player's Current stamina 
	float CurrentStamina;

	// Whether the player's stamina is 0.
	bool bIsExhausted;

	// Player's maximum stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stamina", meta = (AllowPrivateAccess = "true"))
	float MaxStamina;

	//How quickly the player regenerates Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaIncrementRate;

	//How quickly the player drains Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaDecrementRate;

	//Once the player is exhausted, their Stamina must reach this threshold (% of Stamina) before they can sprint again
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stamina", meta = (AllowPrivateAccess = "true"))
	float ExhaustionThreshold;

// Death and Respawn
private:
	// Virtual function called by HealthComponent on death.
	virtual void OnDeath() override;

	UUserWidget* DeathWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> DeathWidgetClass;

	// Saturation Color for the camera when the player dies.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Misc", meta = (AllowPrivateAccess = "true"))
	FVector4 DeathCameraSaturation;

	// Location to spawn the player after respawn.
	FVector RespawnLocation;

	// Removes the death widget and starts Loading the game back again. (See OnRespawn for Respawn Logic)
	UFUNCTION(BlueprintCallable)
	void StartRespawning();

	// Enables/Disables MouseCursor and MouseClick and MouseOver events
	void SetEnableMouse(bool bState);

public:
	bool bWasDead;

	// This function handles the respawning and gets called inside GameInstance.cpp
	void OnRespawn();
	// Set RespawnLocation to NewLocation. Called from Checkpoint.
	void SetSpawnLocation(FVector NewLocation);

// Lighter
public:
	// whether the player is holding the lighter
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	bool bHoldingLighter;

	void HoldLighter();

private:
	void StartRemovingLighter();
	void RemoveLighter();

	// whether the player is finished putting the lighter on his pocket.
	bool bFinishedRemovingLighter;
	// TimerHandle for the removing lighter.
	FTimerHandle LighterTimerHandle;

public:
	/* This variable is to prevent player from endlessly looping because
	the player will be still looking at the trigger box while the LookAt rotation is happening.
	This is done by TempTrigger != Trigger condition */
	APTriggerBase* TempTrigger;

	// Player Controller reference.
	APPlayerController* PlayerController;

// Some Inputs
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	float MouseSensitivity = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	float DoorOpenMouseSensitivity = 1;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void Turn(float Value);
	void LookUp(float Value);
	float TurnRate;

	void DoCrouch();
	void DoUnCrouch();

	void DoJump();

public:
	// Whether the player is crouching
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	float CrouchSpeed;

// Sound Effects
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* SprintBreathLight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* SprintBreathHeavy;

protected:
	UAudioComponent* AudioComp;
	
// Save and Load
public:

	APPlayerController* GetPlayerController() { return PlayerController; }

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	void LoadGame();

	void DeleteSaveGame();

private:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	void LineTraceSightTrigger();
	void LineTraceInteractables();

	void UpdateStamina(float DeltaTime);
	void UpdateSFX(float DeltaTime);
};
