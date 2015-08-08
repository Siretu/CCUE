// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"
#include "Ship.h"
#include "AIController.h"
#include "CruiserCommandCharacter.h"
#include "CCPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API ACCPlayerController : public APlayerController
{
	GENERATED_BODY()

	ACCPlayerController(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(Replicated)
	FTransform targetPos;		// Local position on the ship to path towards.
	class APlayerCamera* camera;

	TSubclassOf<AActor> CharacterClass;

	AAIController* Control;	// Controller of character. Only exists server-side

	UPROPERTY(Replicated)
	ACruiserCommandCharacter* AttachedPawn; // Pawn controlled by this character. Not possessed directly, but possessed by the AIController.

	

	bool bControllingShip;

	virtual void BeginPlay() override;
	virtual void BeginPlayingState() override;
	virtual void SetupInputComponent() override;
	void Tick(float DeltaTime);

	/** Sets up the player camera. Spawns the camera class and sets the view target */
	void SetupCamera();

	/** Called when you right click in the world to order the character to move */
	void OrderMove();
	
	void PlayerCameraCancel();

	/** Navigate player to the given world location (Server Version) */
	UFUNCTION(Unreliable, Server, WithValidation)
	void ServerSetNewMoveDestination(const FVector DestLocation);
	virtual void ServerSetNewMoveDestination_Implementation(const FVector DestLocation);
	virtual bool ServerSetNewMoveDestination_Validate(const FVector DestLocation);

	/** Set marine target position. The Vector component is the local position of the order. The rotation was the ship's rotation at the time of the order. */
	UFUNCTION(Reliable, Server, WithValidation)
	void SetTargetPos(FTransform pos);
	virtual void SetTargetPos_Implementation(FTransform pos);
	virtual bool SetTargetPos_Validate(FTransform pos);

	// Player camera functions. These exist because binding an axis input directly to the camera function seemed to fail.
	void PlayerZoomIn();
	void PlayerZoomOut();
	void PlayerCameraForward(float f);
	void PlayerCameraRight(float f);

	AShip* GetCurrentShip();
};
