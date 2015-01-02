// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "UnrealNetwork.h"
#include "AIController.h"
#include "CruiserCommandCharacter.h"
#include "CCPlayerController.h"
#include "PlayerProxy.generated.h"

/** This class will work as a proxy on the client - tracking the movements of the
* real Character on the server side and sending back controls. */
UCLASS()
class CRUISERCOMMAND_API APlayerProxy : public APawn
{
	GENERATED_UCLASS_BODY()

public:

	// Needed so we can pick up the class in the constructor and spawn it elsewhere
	TSubclassOf<AActor> CharacterClass;

	// Pointer to the actual character. We replicate it so we know its location for the camera on the client.
	UPROPERTY(Replicated)
	ACruiserCommandCharacter* Character;

	// The AI Controller we will use to auto-navigate the player
	AAIController* PlayerAI;

	// We spawn the real player character and other such elements here
	virtual void BeginPlay() override;

	// Use do keep this actor in sync with the real one 
	void Tick(float DeltaTime);

	// Used by the controller to get moving to work 
	void MoveToLocation(const ACCPlayerController* controller, const FVector& vector);

	//virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;
};
