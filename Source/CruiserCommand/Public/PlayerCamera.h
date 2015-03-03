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

	APlayerCamera();
	
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	// Imagine the camera floating on a sphere centered around the capsule component.
	float CameraRadius;			// Sphere radius
	float CameraZAngle;			// Camera angle
	float CameraHeightAngle;	// Camera's angle on the sphere
	float ZoomSpeed;			// Speed at which you can zoom in and out
	float MinRadius;			// CameraRadius can't go below this by zooming in
	float MaxRadius;			// CameraRadius can't go above this by zooming out
	float MovementSpeed;		// Speed at which the camera moves.
	float EdgeForwardAxis;		// Current forward/backward movement from moving the camera to the edges.
	float EdgeRightAxis;		// Current left/right movement from moving the camera to the edges.

	/** Updates the camera relative location and rotation according to the current camera settings */
	void UpdateLocRot();

	/** Returns a vector to a position of the camera sphere */
	FVector PointOnSphere();

	/** Zooms the camera in by reducing the camera radius */
	void ZoomIn();
	/** Zooms the camera out by increasing the camera radius */
	void ZoomOut();
	/** Moves the camera forward or backward depending on the parameter and the value of EdgeForwardAxis */
	void CameraMove(float f, EAxis::Type);
};
