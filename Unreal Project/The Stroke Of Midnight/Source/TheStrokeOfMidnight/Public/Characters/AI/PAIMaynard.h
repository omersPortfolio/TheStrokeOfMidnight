//Copyright 2021 Memento Game Studios. All Rights Reserved.
// Written by Omer Kocar

/*Changelog:

	02/02/21
		- Maynard created
	02/09/21
		- Patrol Path added
	02/13/21
		- Maynard overhauled, renamed from PNPC to PAICharacter
		- Blueprint prototyping moved to C++ code
		- Patrol functionality restored
		- PatrolPath member added
		- GetPatrolPath function added
	02/15/21
		- Separate collider for hand added (for attacking)
		- bCanEverTick turned to false
		- Attack Range member added
		- bool to check if Attacking added
	02/16/21
		- Renamed from PAICharacter to PAIMaynard
		- AI now inherit from EnemyBase class, which inherits from CharacterBase
		- Character movement activations and deactivation added
	02/21/21
		- Player pointer moved to EnemyBase
	02/25/21
		- Moved hand collider's OnOverlap subscription to the constructor (from BeginPlay)
		- Removed the Player pointer that wasn't removed when moving it to the EnemyBase class, causing a game-breaking issue
	02/26/21
		- PatrolPath restored
		- Various comments added
	03/14/21
		- Damage reduced from 100 to 50 (one-hit kill to a two-hit kill)
	03/15/21
		- Added new comments
		- Added a new variable named bAttacked.

*/

#pragma once

#include "CoreMinimal.h"
#include "PEnemyBase.h"
#include "PAIMaynard.generated.h"

class APPlayer;
class APPatrolPath;
class UCapsuleComponent;


UCLASS()
class THESTROKEOFMIDNIGHT_API APAIMaynard : public APEnemyBase
{
	GENERATED_BODY()

public:
	APAIMaynard();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* HandCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	TSubclassOf<UDamageType> DamageClass;

	// Whether Maynard already attacked. This is set to true in BeginOverlap and to false in Attack interface function (see the Blueprint).
	UPROPERTY(BlueprintReadWrite, Category = "NPC")
	bool bAttacked;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	APPatrolPath* PatrolPath;

	

public:

	//Deactivates movement when attacking
	UFUNCTION(BlueprintCallable)
	void DeactivateMovement();

	// Activates movements when attacking ends
	UFUNCTION(BlueprintCallable)
	void ActivateMovement();

	APPatrolPath* GetPatrol() { return PatrolPath; }

private:
	// Maynard's hand's collider to check if it hits the player while attacking.
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
