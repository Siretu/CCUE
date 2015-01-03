// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
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
	FVector targetPos;

	virtual void BeginPlay() override;
	virtual void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;

	class APlayerCamera* camera;

	void SetupCamera();
	virtual void SetupInputComponent() override;

	/** Called when you right click in the world to order the character to move */
	void OrderMove();

	
	/** Navigate player to the given world location (Server Version) */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetNewMoveDestination(const FVector DestLocation);

	void PlayerTick(float DeltaTime);

	void PlayerZoomIn();
	void PlayerZoomOut();

	void CameraForward(float f);
	void CameraRight(float f);
};
