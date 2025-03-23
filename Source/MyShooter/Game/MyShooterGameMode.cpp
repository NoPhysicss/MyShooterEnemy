// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyShooterGameMode.h"
#include "MyShooterPlayerController.h"
#include "MyShooter/Cherecter/MyShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyShooterGameMode::AMyShooterGameMode()
{
	/* // use our custom PlayerController class
	PlayerControllerClass = AMyShooterPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BluePrint/Charecter/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/BluePrint/Charecter/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}*/
}

void AMyShooterGameMode::PlayerCharacterDead()
{

}
