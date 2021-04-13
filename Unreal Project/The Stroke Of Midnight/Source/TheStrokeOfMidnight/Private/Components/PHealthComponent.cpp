//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Zoe Purcell


#include "Components/PHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PCharacterBase.h"


// Sets default values for this component's properties
UPHealthComponent::UPHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Default values. All modifiable in the Owner
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bCanRegenerate = true;
	bIsRegenerating = false;
	RegenerationDelay = 5.0f;
	RegenerationIncrementRate = 5.0f;

	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UPHealthComponent::TakeDamage);
	}
}

void UPHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Check if the Owner is missing health and is not already dead
	if (CurrentHealth < MaxHealth && !IsDead())
	{
		//Check if Owner is able to regenerate health, and isn't already doing so
		if (bCanRegenerate && !bIsRegenerating)
		{
			//After a short delay, start regenerating health
			FTimerHandle RegenHandle;
			GetOwner()->GetWorldTimerManager().SetTimer(RegenHandle, this, &UPHealthComponent::BeginRegeneration, RegenerationDelay, false);
		}

		//Check if Owner is currently regenerating health
		if (bIsRegenerating)
		{
			RegenerateHealth(DeltaTime);
		}
	}
}

bool UPHealthComponent::IsDead()
{
	return CurrentHealth <= 0;
}

// Called when the game starts
void UPHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//Check if Owner isn't already dead
	if (!IsDead())
	{
		//Return if 0 damage is dealt
		if (Damage <= 0)
			return;
		
		//Turn off health regeneration
		bIsRegenerating = false;

		//Remove from Health, ensuring it doesn't go below 0
		CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "Health: " + FString::SanitizeFloat(CurrentHealth));

		//Clear the RegenerationDelay Timer
		//If the Owner takes damage, they have to wait the full duration before their health can regenerate again
		GetOwner()->GetWorldTimerManager().ClearAllTimersForObject(this);

		//Check if this attack killed the Owner
		if (IsDead())
		{
			//If the Owner is a CharacterBase, call their OnDeath function
			if (GetOwner()->IsA(APCharacterBase::StaticClass()))
			{
				APCharacterBase* Character = Cast<APCharacterBase>(GetOwner());
				Character->OnDeath();
			}
		}
	}
}

void UPHealthComponent::BeginRegeneration()
{
	bIsRegenerating = true;
}

void UPHealthComponent::RegenerateHealth(float DeltaTime)
{
	//Add to Health. FMath::Clamp prevents the health from going over the max.
	CurrentHealth = FMath::Clamp(CurrentHealth + (RegenerationIncrementRate * DeltaTime), 0.0f, MaxHealth);
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, "Health: " + FString::SanitizeFloat(CurrentHealth));

	//Turn off health regeneration when health is full
	if (CurrentHealth == MaxHealth)
	{
		bIsRegenerating = false;
	}
}

