// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"
#include "Ship.h"
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
	FVector targetPos;		// Local position on the ship to path towards.
	class APlayerCamera* camera;

	bool bControllingShip;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void PlayerTick(float DeltaTime);
	// Had to override SetViewTarget to prevent it being overwritten at the start of the game.
	virtual void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;

	/** Sets up the player camera. Spawns the camera class and sets the view target */
	void SetupCamera();

	/** Called when you right click in the world to order the character to move */
	void OrderMove();
	
	/** Navigate player to the given world location (Server Version) */
	UFUNCTION(Unreliable, Server, WithValidation)
	void ServerSetNewMoveDestination(const FVector DestLocation);
	virtual void ServerSetNewMoveDestination_Implementation(const FVector DestLocation);
	virtual bool ServerSetNewMoveDestination_Validate(const FVector DestLocation);

	/** Set marine target position */
	UFUNCTION(Reliable, Server, WithValidation)
	void SetTargetPos(FVector pos);
	virtual void SetTargetPos_Implementation(FVector pos);
	virtual bool SetTargetPos_Validate(FVector pos);

	// Player camera functions. These exist because binding an axis input directly to the camera function seemed to fail.
	void PlayerZoomIn();
	void PlayerZoomOut();
	void PlayerCameraForward(float f);
	void PlayerCameraRight(float f);

	AShip* GetCurrentShip();
};
