// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Ship.generated.h"


// Forward Declarations
class ACruiserCommandCharacter;

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API AShip : public APawn
{
	GENERATED_BODY()

	AShip(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void Tick(float delta) override;

	void MoveForward();
	void StopMoveForward();
	void EnterShip(ACruiserCommandCharacter* character);

	// Variables
	bool bMovingForward;
	float RotationSpeed;
	float MovementSpeed;
	FVector EnterPosition; // When a character enters the ship, it will be transferred to this position.

protected:
	FRotator TargetRotation;

	// Getters & Setters
public:
	//FRotator GetTargetRotation();
	void SetTargetRotation(FRotator newRot);
};
