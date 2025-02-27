// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "PlayerCamera.h"


APlayerCamera::APlayerCamera() {

	// Initialize components
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(ACharacter::CapsuleComponentName);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	UE_LOG(LogTemp, Warning, TEXT("Camera constructed"));

	// Use capsule as root and parent for the follow camera.
	RootComponent = CapsuleComponent;
	FollowCamera->AttachParent = RootComponent;

	CameraRadius = 1024;
	CameraZAngle = 0;
	CameraHeightAngle =  -3.1415 / 6;
	ZoomSpeed = 64;
	MinRadius = 128;
	MaxRadius = 4096;
	MovementSpeed = 15;
	EdgeForwardAxis = 0;
	EdgeRightAxis = 0;

	UpdateLocRot();
}

/** Updates the camera relative location and rotation according to the current camera settings */
void APlayerCamera::UpdateLocRot() {
	// Set location
	FVector newLocation = PointOnSphere();
	FollowCamera->SetRelativeLocation(newLocation);

	// Set rotation
	FRotator rot = FRotationMatrix::MakeFromX(FVector(0, 0, 0) - FollowCamera->GetRelativeTransform().GetLocation()).Rotator();
	FollowCamera->SetRelativeRotation(rot);
}

/** Returns a vector to a position of the camera sphere */
FVector APlayerCamera::PointOnSphere() {
	float x = CameraRadius * cos(CameraZAngle) * sin(CameraHeightAngle);
	float y = CameraRadius * sin(CameraZAngle) * sin(CameraHeightAngle);
	float z = CameraRadius * cos(CameraHeightAngle);

	FVector result(x, y, z);
	return result;
}

void APlayerCamera::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CameraMove();
}

/** Zooms the camera in by reducing the camera radius */
void APlayerCamera::ZoomIn() {
	// Make sure you don't zoom in too far
	CameraRadius = FMath::Clamp(CameraRadius - ZoomSpeed, MinRadius, MaxRadius);
	UpdateLocRot();
}

/** Zooms the camera out by increasing the camera radius */
void APlayerCamera::ZoomOut() {
	// Make sure you don't zoom out too far
	CameraRadius = FMath::Clamp(CameraRadius + ZoomSpeed, MinRadius, MaxRadius);
	UpdateLocRot();
}

/** Moves the camera forward or backward depending on the parameter and the value of EdgeForwardAxis */
void APlayerCamera::CameraMove() {

	// Get just the camera yaw
	FRotator worldRot = FollowCamera->GetComponentRotation();
	FRotator isolatedYaw = FRotator(0, worldRot.Yaw, 0);
	
	// Add the yaw vector multiplied by input and movement speed to the current location.
	FVector newL = GetActorLocation() + FRotationMatrix(isolatedYaw).GetScaledAxis(EAxis::X) * MovementSpeed * EdgeForwardAxis;
	newL = newL + FRotationMatrix(isolatedYaw).GetScaledAxis(EAxis::Y) * MovementSpeed * EdgeRightAxis;
	SetActorLocation(newL);
}