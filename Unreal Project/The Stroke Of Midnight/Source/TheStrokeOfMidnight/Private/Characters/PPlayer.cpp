//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar and Zoe Purcell

#include "Characters/PPlayer.h"
#include "Camera/CameraComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include "Components/PointLightComponent.h"
#include <Components/BoxComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "Interactables/PInteractableBase.h"
#include "Interactables/PInteractableDoor.h"
#include <Particles/ParticleSystemComponent.h>
#include "Triggers/PTriggerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PPlayerController.h"
#include "Components/AudioComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/PHealthComponent.h"
#include "Framework/PGameInstance.h"
#include <Blueprint/UserWidget.h>

APPlayer::APPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	Init();
}

void APPlayer::Init()
{
	// Initialize Components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetMesh(), FName("Head"));

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	LighterMesh = CreateDefaultSubobject<UStaticMeshComponent>("Lighter");
	LighterMesh->SetupAttachment(GetMesh(), FName("LighterSocket"));
	LighterMesh->SetVisibility(false);

	LighterLight = CreateDefaultSubobject<UPointLightComponent>("LighterLight");
	LighterLight->SetupAttachment(LighterMesh);
	LighterLight->SetVisibility(false);

	LighterFlame = CreateDefaultSubobject<UParticleSystemComponent>("LighterFlame");
	LighterFlame->SetupAttachment(LighterMesh);
	LighterFlame->SetActive(false);
	LighterFlame->SetAutoActivate(false);

	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	// Initialize variables
	WalkSpeed = 200.0f;
	SprintSpeed = 350.0f;
	CrouchSpeed = 150.0f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bCrouching = false;
	bHoldingLighter = false;
	bFinishedRemovingLighter = true;
	InteractionRange = 150.0f;
	bCanSprint = false;

	bIsExhausted = false;
	MaxStamina = 200.0f;
	CurrentStamina = MaxStamina;
	StaminaIncrementRate = 10.0f;
	StaminaDecrementRate = 10.0f;
	ExhaustionThreshold = MaxStamina * 0.25;
	StaminaState = EStaminaState::Regenerating;
	SprintNoiseRange = 500.0f;

	DeathCameraSaturation = FVector(0.3, 0.3, 0.3);
}

void APPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Find the PlayerController
	PlayerController = Cast<APPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	// Set the RespawnLocation to Player's Start Location.
	RespawnLocation = GetActorLocation();
}

void APPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	UpdateStamina(DeltaTime);
	UpdateSFX(DeltaTime);

	LineTraceSightTrigger();
	LineTraceInteractables();

}

void APPlayer::LineTraceSightTrigger()
{
	// Triggers Look At when the player Looks at a Look At Trigger with LookTrace in a range. 
	// (Currently 300 but will change it to be adjustable for each trigger)
	FHitResult Hit;
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * 300.0f;

	// If an object that has LookTrace is traced.
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1))
	{
		// Get a reference to the Trigger that's looked at.
		APTriggerBase* Trigger = Cast<APTriggerBase>(Hit.Actor);


		if (Trigger && TempTrigger != Trigger)
		{
			// Call the Trigger's Event function.
			Trigger->Event(this, this);
			// Set tempTrigger to Trigger to prevent multiple triggers.
			TempTrigger = Trigger;
		}
	}
}

void APPlayer::LineTraceInteractables()
{
	FHitResult Hit1;
	FVector Start1 = Camera->GetComponentLocation();
	FVector End1 = Start1 + Camera->GetForwardVector() * InteractionRange;

	// if an object is traced through InteractableTrace channel.
	if (GetWorld()->LineTraceSingleByChannel(Hit1, Start1, End1, ECC_GameTraceChannel2))
	{
		bTracedInteractable = true;
	}
	else
	{
		bTracedInteractable = false;
	}
}

void APPlayer::UpdateStamina(float DeltaTime)
{
	// Stamina Logic
	switch (StaminaState)
	{
	case EStaminaState::Draining:

		//Check if player is moving
		if (GetVelocity().Size() > 0)
		{
			// Clamp CurrentStamina - (StaminaDecrementRate * DeltaTime) between 0 and MaxStamina.
			CurrentStamina = FMath::Clamp(CurrentStamina - (StaminaDecrementRate * DeltaTime), 0.0f, MaxStamina);
			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, "Stamina: " + FString::SanitizeFloat(CurrentStamina));
			// if stamina is 0
			if (CurrentStamina == 0.0f)
			{
				// set stamina to 0
				CurrentStamina = 0.0f;
				// Stop Sprinting
				StopSprint();
				// Set bIsExhausted to true.
				bIsExhausted = true;

				GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Red, "Exhausted!");
			}
		}
		break;
	case EStaminaState::Regenerating:
		// Clamp CurrentStamina + (StaminaIncrementRate * DeltaTime) between 0 and MaxStamina
		CurrentStamina = FMath::Clamp(CurrentStamina + (StaminaIncrementRate * DeltaTime), 0.0f, MaxStamina);
		// if stamina is lower than MaxStamina
		if (CurrentStamina < MaxStamina)
			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, "Stamina: " + FString::SanitizeFloat(CurrentStamina));
		// if stamina is greater than or equal to ExhaustionThreshold
		if (CurrentStamina >= ExhaustionThreshold)
		{
			// Set bIsExhausted to false
			bIsExhausted = false;
		}
		break;
	default:
		break;
	}

	//If the player isn't moving forward anymore, OR input is disabled, stop sprinting.
	if (!PlayerController->InputEnabled() || !PlayerController->IsInputKeyDown(FKey("W")))
	{
		StopSprint();
	}
}

void APPlayer::UpdateSFX(float DeltaTime)
{
	//Sprinting SFX
	if (!Execute_IsDead(this))
	{
		// If Stamina is full
		if (CurrentStamina == MaxStamina)
		{
			if (AudioComp->IsPlaying())
			{
				AudioComp->FadeOut(0.5f, 0.0f);
			}
		}
		// If your Stamina is below full, but over 33%
		if (CurrentStamina < MaxStamina && CurrentStamina > MaxStamina * 0.33)
		{
			// Fade out any other sprint sound if it was already playing
			if (AudioComp->IsPlaying() && AudioComp->Sound != SprintBreathLight)
			{
				AudioComp->FadeOut(0.3f, 0.0f);
			}
			if (!AudioComp->IsPlaying())
			{
				AudioComp->SetSound(SprintBreathLight);
				AudioComp->FadeIn(0.2f, 1.0f);
			}
		}
		// If your Stamina is below 33%
		if (CurrentStamina < MaxStamina * 0.25)
		{
			if (AudioComp->IsPlaying() && AudioComp->Sound != SprintBreathHeavy)
			{
				AudioComp->FadeOut(0.3f, 0.0f);
			}
			if (!AudioComp->IsPlaying())
			{
				AudioComp->SetSound(SprintBreathHeavy);
				AudioComp->FadeIn(0.2f, 1.0f);
			}
		}
	}
}

void APPlayer::Interact()
{
	// Setup data for Linetrace
	FHitResult Hit;
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * InteractionRange;

	// if an object is traced through InteractableTrace channel.
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel2))
	{
		// Get a reference to the traced interactable object.
		APInteractableBase* Obj = Cast<APInteractableBase>(Hit.Actor);
		// Get a reference to the Box Component of it.
		UBoxComponent* Collision = Cast<UBoxComponent>(Hit.Component);

		// if Collision is valid
		if (Collision)
		{
			// Call the Interact function of the Interactable object.
			Obj->Interact();
		}
	}

}

void APPlayer::HoldLighter()
{
	// if not Holding Lighter, finished removing lighter and not sprinting.
	if (!bHoldingLighter)
	{
		if (bFinishedRemovingLighter && !IsSprinting())
		{
			// Set bHoldingLighter to true.
			bHoldingLighter = true;
			// Set bFinishedRemovingLighter to false.
			bFinishedRemovingLighter = false;

			// Activate LighterMesh and Flame and set them to visible.
			LighterMesh->SetVisibility(true);
			LighterFlame->SetActive(true);
			LighterFlame->SetVisibility(true);
			LighterLight->SetVisibility(true);
		}
	}
	// if Holding Lighter
	else
	{
		// Set bHoldingLighter to false
		bHoldingLighter = false;
		// Set a timer for RemoveLighter function
		GetWorldTimerManager().SetTimer(LighterTimerHandle, this, &APPlayer::RemoveLighter, 0.3f, false);
	}
}

void APPlayer::StartRemovingLighter()
{
	// set bHoldingLighter to false
	bHoldingLighter = false;
	// Set a timer for RemoveLighter.
	GetWorldTimerManager().SetTimer(LighterTimerHandle, this, &APPlayer::RemoveLighter, 0.3f, false);

}

void APPlayer::RemoveLighter()
{
	// Deactivate LighterMesh and Flame and set them to not visible.
	LighterFlame->SetActive(false);
	LighterFlame->SetVisibility(false);
	LighterMesh->SetVisibility(false);
	LighterLight->SetVisibility(false);

	// Set bFinishedRemovingLighter to true
	bFinishedRemovingLighter = true;
	// Clear all timer for this object.
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void APPlayer::AddKey(int KeyChannel)
{
	// Add a KeyChannel integer to the KeyChannels list.
	KeyChannels.AddUnique(KeyChannel);
}

bool APPlayer::HasKey(int KeyChannel) const
{
	// Whether KeyChannels has the KeyChannel integer.
	return KeyChannels.Contains(KeyChannel);
}

void APPlayer::MoveForward(float Value)
{

	// Move Forward Logic
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);

	// if Value is greater than 0
	if (Value > 0)
	{
		// Set bCanSprint to true
		bCanSprint = true;
	}
	else
	{
		// Set bCanSprint to false
		bCanSprint = false;
	}

}

void APPlayer::MoveRight(float Value)
{

	// Move Right logic
	bCanSprint = true;
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);

}

void APPlayer::Turn(float Value)
{
	if (Value != 0)
	{
		TurnRate = Value * MouseSensitivity;
		AddControllerYawInput(TurnRate);
	}
}

void APPlayer::LookUp(float Value)
{
	if (Value != 0)
	{
		AddControllerPitchInput(Value * MouseSensitivity);
	}
}

void APPlayer::DoCrouch()
{
	GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
	Crouch();
	// Set bCrouching to true
	bCrouching = true;
}

void APPlayer::DoUnCrouch()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	UnCrouch();
	// Set bCrouching to false
	bCrouching = false;
}

void APPlayer::DoJump()
{

	Jump();
}

void APPlayer::Sprint()
{
	if (!bCrouching && bCanSprint && !bIsExhausted)
	{

		// Adjust movement speed and Stamina state
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		StaminaState = EStaminaState::Draining;

		// Noise is disabled due to Maynard's Attacking state not overriding other States. will fix later
		// Make Noise to trigger AI hearing stimuli
		//MakeNoise(1.0f, this, GetActorLocation(), SprintNoiseRange, FName("Player"));

		if (bHoldingLighter)
		{
			StartRemovingLighter();
		}
	}
}

void APPlayer::StopSprint()
{
	// Set the movement speed to walkspeed 
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	// Set the staminaState to Regenerating.
	StaminaState = EStaminaState::Regenerating;
}

bool APPlayer::IsSprinting()
{
	return StaminaState == EStaminaState::Draining;
}

void APPlayer::OnDeath()
{
	if (DeathWidget)
	{
		DeathWidget->RemoveFromParent();
	}
	DeathWidget = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(), 0), DeathWidgetClass);
	DeathWidget->AddToViewport(0);


	// Set ticking to false
	PrimaryActorTick.bCanEverTick = false;

	// Set bUsePawnControlRotation to false on Camera.
	Camera->bUsePawnControlRotation = false;

	// Disable PlayerController Input.
	DisableInput(PlayerController);

	// Change Camera's Saturation Color for death screen.
	Camera->PostProcessSettings.bOverride_ColorSaturation = true;
	Camera->PostProcessSettings.ColorSaturation = FVector4(DeathCameraSaturation, 1);

	// Set EnableMouse to true.
	SetEnableMouse(true);

	bWasDead = true;
}

void APPlayer::StartRespawning()
{
	// if we are dead
	if (HealthComponent->IsDead())
	{
		DeathWidget->RemoveFromParent();

		if (UGameplayStatics::DoesSaveGameExist(GameInstance->SaveSlotName, 0))
		{
			// Load the game.
			LoadGame();
		}
	}
}

void APPlayer::OnRespawn()
{
	// Set the CurrentHealth to MaxHealth
	HealthComponent->CurrentHealth = HealthComponent->MaxHealth;
	// Set CurrentStamina to MaxStamina
	CurrentStamina = MaxStamina;

	// Bring back ticking again.
	PrimaryActorTick.bCanEverTick = true;
	// Set bUsePawnControlRotation to true on Camera.
	Camera->bUsePawnControlRotation = true;
	// Enable PlayerController input.
	EnableInput(PlayerController);

	// Revert the Camera's Saturation color to normal
	Camera->PostProcessSettings.bOverride_ColorSaturation = false;
	Camera->PostProcessSettings.ColorSaturation = FVector4(1, 1, 1, 1);

	// Set EnableMouse to false.
	SetEnableMouse(false);

	bWasDead = false;
}

void APPlayer::SetSpawnLocation(FVector NewLocation)
{
	// Set RespawnLocation to NewLocation.
	RespawnLocation = NewLocation;
}

void APPlayer::SetEnableMouse(bool bState)
{
	// Set show mouse cursor to bState
	PlayerController->bShowMouseCursor = bState;
	// Set bEnableClickEvents to bState
	PlayerController->bEnableClickEvents = bState;
	// Set bEnableMouseOverEvents to bState
	PlayerController->bEnableMouseOverEvents = bState;
}

#pragma region Save and Load

void APPlayer::SaveGame()
{
	GameInstance->SaveGame();
}

void APPlayer::LoadGame()
{
	GameInstance->LoadGame();
}

void APPlayer::DeleteSaveGame()
{
	GameInstance->DeleteSaveData();
}

#pragma endregion

// Called to bind functionality to input
void APPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Assertion for PlayerInputComponent's validation
	check(PlayerInputComponent);

	// Input Bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &APPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APPlayer::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APPlayer::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APPlayer::DoJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APPlayer::DoCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APPlayer::DoUnCrouch);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APPlayer::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APPlayer::StopSprint);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APPlayer::Interact);

	PlayerInputComponent->BindAction("Lighter", IE_Pressed, this, &APPlayer::HoldLighter);

	PlayerInputComponent->BindAction("Save", IE_Pressed, this, &APPlayer::SaveGame);
	PlayerInputComponent->BindAction("Load", IE_Pressed, this, &APPlayer::LoadGame);
	PlayerInputComponent->BindAction("Delete", IE_Pressed, this, &APPlayer::DeleteSaveGame);
}
