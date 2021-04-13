//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	01/30/21
		- Transfer of files to new repository
	02/09/21
		- Class set as Abstract
	03/10/21
		- Removed InteractionSphere
		- Removed TextRender
	03/14/21
		- Added new comments
		- Moved forward declarations to the top of the header.

*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "PInteractableBase.generated.h"

class APPlayer;
class UBoxComponent;
class UPGameInstance;

// This class is Abstract.
UCLASS(Abstract)
class THESTROKEOFMIDNIGHT_API APInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APInteractableBase();

	// Player reference
	APPlayer* Character;

public:	
	// Pure virtual Interact function that's called from Player's Interact function.
	virtual void Interact() PURE_VIRTUAL(AFInteractableBase::Interact, );

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	inline FGuid GetGuid() { return Guid; }	

protected:
	virtual void BeginPlay() override;

	FGuid Guid;
};
