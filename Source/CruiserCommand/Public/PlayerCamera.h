// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PlayerCamera.generated.h"

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API APlayerCamera : public APawn
{
	GENERATED_BODY()

	APlayerCamera(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	float CameraRadius;
	float CameraZAngle;
	float CameraHeightAngle;
	float ZoomSpeed;
	float MinRadius;
	float MaxRadius;
	float MovementSpeed;
	float EdgeForwardAxis;
	float EdgeRightAxis;

	void UpdateLocRot();

	FVector PointOnSphere();

	void ZoomIn();
	void ZoomOut();
	void CameraMove(float f, EAxis::Type);
};
