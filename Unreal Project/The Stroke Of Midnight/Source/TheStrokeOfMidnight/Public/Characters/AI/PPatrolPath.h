//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar

/*Changelog:

	02/13/21
		- Patrol Path created
	03/04/21
		- Forward declaration moved to top of the file
	03/15/21
		- Added new comments

*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPatrolPath.generated.h"

class USplineComponent;

UCLASS()
class THESTROKEOFMIDNIGHT_API APPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPatrolPath();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* PatrolPath;

	// Increments the spline path index.
	UFUNCTION(BlueprintCallable)
	void IncrementPatrolIndex();

	// Returns the next spline point as World Position
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetNextPointAsWorldPos();

	void SetPatrolIndex(int newIndex);

	// Patrol path index
	int PatrolIndex;
};
