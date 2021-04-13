//Copyright 2021 Memento Game Studios. All Rights Reserved.
//Written by Omer Kocar and Zoe Purcell

/*Changelog:

	02/16/21
		- Base Enemy class added
	02/21/21
		- Player pointer added
		- Attack Range float added

*/

#pragma once

#include "CoreMinimal.h"
#include "Characters/PCharacterBase.h"
#include "PEnemyBase.generated.h"

class APPlayer;

UCLASS()
class THESTROKEOFMIDNIGHT_API APEnemyBase : public APCharacterBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float AttackRange;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APPlayer* Player;
};
