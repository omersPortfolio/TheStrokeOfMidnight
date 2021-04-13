//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	01/30/21
		- Transfer of files to new repository

	03/14/21
		- Removed BeginPlay and Tick
		- Set bCanEverTick to false

*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPhysicsDoor.generated.h"

class UBoxComponent;

UCLASS()
class THESTROKEOFMIDNIGHT_API APPhysicsDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPhysicsDoor();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FrameMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComp;
};
