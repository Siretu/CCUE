// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "CruiserCommand.h"
#include "CruiserCommandGameMode.h"
#include "CruiserCommandCharacter.h"
#include "PlayerCamera.h"
#include "CCPlayerController.h"
#include "PlayerProxy.h"
#include "CCHUD.h"

ACruiserCommandGameMode::ACruiserCommandGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Proxy character
	DefaultPawnClass = APlayerCamera::StaticClass();

	// Use our custom PlayerController class
	PlayerControllerClass = ACCPlayerController::StaticClass();
	incrementId = 0;
	// Use our custom HUD class
	HUDClass = ACCHUD::StaticClass();	
}

void ACruiserCommandGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	if (Role == ROLE_Authority)	{
		UE_LOG(LogTemp, Warning, TEXT("POSTLOGIN AUTH"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("POSTLOGIN NO AUTH"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Gamemode postlogin"));

	NewPlayer->PlayerState->PlayerId = incrementId;
	incrementId++;

	UE_LOG(LogTemp, Warning, TEXT("PlayerController Playerid%d"), NewPlayer->PlayerState->PlayerId);
}