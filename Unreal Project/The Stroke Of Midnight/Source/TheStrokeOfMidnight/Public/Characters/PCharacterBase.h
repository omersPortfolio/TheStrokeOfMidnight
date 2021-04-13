//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	02/15/21
		- Base Character class added
	08/04/21
		- Added GetHealthComponent
		- Added Guid as a unique ID.

*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/AI/INT_AIHelper.h"
#include "PCharacterBase.generated.h"

class UPHealthComponent;
class UPGameInstance;

UCLASS()
class THESTROKEOFMIDNIGHT_API APCharacterBase : public ACharacter, public IINT_AIHelper
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APCharacterBase();

	// The parent OnDeath function that HealthComponent calls on Character death.
	virtual void OnDeath();

	virtual void BeginPlay() override;

	FVector SpawnLocation;

	UPROPERTY(BlueprintReadOnly)
	UPGameInstance* GameInstance;

	inline UPHealthComponent* GetHealthComponent() { return HealthComponent; }

	inline FGuid GetGuid() { return Guid; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Health")
	UPHealthComponent* HealthComponent;

	FGuid Guid;
};
