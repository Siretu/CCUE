// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "CruiserCommand.h"
#include "CruiserCommandGameMode.h"
#include "CruiserCommandCharacter.h"
#include "CCPlayerController.h"
#include "PlayerProxy.h"

ACruiserCommandGameMode::ACruiserCommandGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/

	DefaultPawnClass = APlayerProxy::StaticClass();

	// Use our custom PlayerController class
	PlayerControllerClass = ACCPlayerController::StaticClass();
}
