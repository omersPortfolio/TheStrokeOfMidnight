//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Zoe Purcell

/*Changelog:

	02/21/21
		- Statue AI created
	02/23/21
		- Statue now smoothly rotates to player's direction rather than snapping
	03/14/21
		- Added comments for Alpha 1 submission
		- Replaces OtherActor->IsA(PPlayer) with OtherActor == Player, to use the Player pointer in EnemyBase
	03/28/21
		- Added sound effect for statue's movement
		- Added sound effect for statue's turning
		- Added an AudioComponent to play and stop these sound effects when movement/rotation starts and stops
		- Added FSoundAttenuationSettings to the sounds will fall off when the player is far away
		- Added a LastRotation FRotator to properly check if the statue is rotating 
*/

#pragma once

#include "CoreMinimal.h"
#include "PEnemyBase.h"
#include "PAIStatue.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class THESTROKEOFMIDNIGHT_API APAIStatue : public APEnemyBase
{
	GENERATED_BODY()

public:
	APAIStatue();

	//A small box that extends in front of the Statue. If the player overlaps these bounds, they take damage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* KillTrigger;

	// Sound effect that plays when the statue is moving towards the player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		USoundBase* MovementSound;

	// Sound effect that plays when the statue is rotating to face the player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		USoundBase* TurnSound;
	
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

protected:
	// Used to check whether the statue is currently rotating or not (to play the sound effect)
	FRotator LastRotation;

	// Needed to have the sound effects get quieter the farther the player is, when using an AudioComponent
	FSoundAttenuationSettings AttenSettings;\
	
	UAudioComponent* AudioComp;
};
