//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar


#include "Misc/PPhysicsDoor.h"
#include <Components/SceneComponent.h>
#include <Components/BoxComponent.h>

// Sets default values
APPhysicsDoor::APPhysicsDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = RootComp;

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>("FrameMesh");
	FrameMesh->SetupAttachment(RootComp);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorMesh");
	DoorMesh->SetupAttachment(FrameMesh);
	DoorMesh->SetSimulatePhysics(false);

	CollisionComp = CreateDefaultSubobject<UBoxComponent>("HingeCollision");
	CollisionComp->SetupAttachment(RootComp);
	CollisionComp->SetAllUseCCD(true);
	CollisionComp->SetRelativeLocation(FVector(0, 65.0f, 105.0f));
	CollisionComp->SetBoxExtent(FVector(20.0f, 9.0f, 95.0f));
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	CollisionComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
}

