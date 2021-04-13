//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Zoe Purcell and Omer Kocar

/*Changelog:

	02/23/21
		- Portals added
		- Fixed an issue where the Player wouldn't be rotated to the exit's forward direction
	03/04/21
		- Forward declarations moved to top of file
	03/10/21
		- Set bCanEverTick to false
		- Removed unused Tick and BeginPlay
	04/01/21
		- Added the whole Portal Effect.

*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPortal.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class APlayerController;
class UCameraShake;
class APPlayer;

UCLASS()
class THESTROKEOFMIDNIGHT_API APPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPortal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* PortalMesh;

	//Assign to an Empty Actor. The player will spawn on this Empty Actor when interacting with the portal. Assign to another Portal if making a 2-way portal.
	UPROPERTY(EditInstanceOnly, Category = "Portal")
	AActor* ExitActor;
	
	//Only implement if we plan on doing 2-way portals. This will add an offset so you don't teleport into another portal's trigger box. Prevents an infinite loop.
	FVector ExitOffsetVector;

	//The distance of the Exit Offset. Only needed for 2-Way portals. Prevents an infinite loop.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	float ExitOffsetDistance;

	FTimerHandle WarpTimerHandle;

	FTimerHandle TeleportTimerHandle;

	FTimerHandle FadeInOutTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	AActor* WarpActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	float FadeInOutTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	float OffsetTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	float TeleportTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	TSubclassOf<UCameraShake> CameraShake;

	UPROPERTY(EditAnywhere, Category = "Portal")
	FName LevelToLoad;

	UPROPERTY(EditAnywhere, Category = "Portal")
	FName LevelToUnload;

private:

	APlayerController* PlayerController = nullptr;
	APPlayer* Player = nullptr;

	void Warp(AActor* OtherActor);

	bool bTeleportTriggered;

	void Teleport(AActor* OtherActor);

	void FadeInOut(AActor* OtherActor);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
