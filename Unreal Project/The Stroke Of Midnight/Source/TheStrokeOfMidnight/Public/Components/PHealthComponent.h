//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Zoe Purcell

/*Changelog:

	02/05/21
		- Health Component created
	02/06/21
		- Component owner no longer immediately regenerates health when health reaches 0
	02/10/21
		- Added bool IsDead function
		- Owner now only takes damage when they are not dead
	02/13/21
		- IsDead is now Blueprint Callable and Pure
	02/15/21
		- Now calls the CharacterBase's OnDeath when health is 0,
	02/21/21
		- IsDead now checks if Health is <= 0, not just = 0
	03/13/21
		- Added more comments for Alpha 1 requirements
		- Health regeneration gets turned off when Health is full, resulting in less operating per tick
	03/14/21
		- Removed turning off bCanRegenerate when dying, as respawning would turn it back on for Characters who didn't have it in the first place
		- Adjusted conditions for regeneration if statements so regeneration is properly turned off
		- Moved Regeneration code from Tick into its own function
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THESTROKEOFMIDNIGHT_API UPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Checks if the Health is 0
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsDead();

	//The maximum health the owner has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float MaxHealth;

	//The current health thr owner has
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		float CurrentHealth;

	//Whether or not the owner can regenerate their health after a short time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool bCanRegenerate;

	//Amount of delay before the owner's health regenerates
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (EditCondition = "bCanRegenerate"))
		float RegenerationDelay;

	//How quickly the owner's health regenerates
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (EditCondition = "bCanRegenerate"))
		float RegenerationIncrementRate;

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Applies Damage by removing from Health, calls owner's OnDeath() if Health is below 0
	UFUNCTION()
		void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//Sets bIsRegenerating to true, allowing Health to regenerate
	UFUNCTION()
		void BeginRegeneration();

	void RegenerateHealth(float DeltaTime);

	//Whether or not Health is currently regenerating
	bool bIsRegenerating;
	
};
