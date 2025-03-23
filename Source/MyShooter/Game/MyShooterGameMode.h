// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyShooterGameMode.generated.h"

UCLASS(minimalapi)
class AMyShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyShooterGameMode();

	void PlayerCharacterDead();
};



