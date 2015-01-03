// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "CruiserCommand.h"
#include "CruiserCommandGameMode.h"
#include "CruiserCommandCharacter.h"
#include "CCPlayerController.h"
#include "PlayerProxy.h"
#include "CCHUD.h"

ACruiserCommandGameMode::ACruiserCommandGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Proxy character
	DefaultPawnClass = APlayerProxy::StaticClass();

	// Use our custom PlayerController class
	PlayerControllerClass = ACCPlayerController::StaticClass();

	// Use our custom HUD class
	HUDClass = ACCHUD::StaticClass();	
}
