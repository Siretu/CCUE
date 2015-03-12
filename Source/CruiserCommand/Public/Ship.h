// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "UnrealNetwork.h"
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
public:
	AShip();

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void Tick(float delta) override;

	UFUNCTION(Reliable, Server, WithValidation)
	void MoveForward();
	virtual bool MoveForward_Validate();
	virtual void MoveForward_Implementation();
	
	UFUNCTION(Reliable, Server, WithValidation)
	void StopMoveForward();
	virtual bool StopMoveForward_Validate();
	virtual void StopMoveForward_Implementation();

	void EnterShip(ACruiserCommandCharacter* character);

	// Variables
	UPROPERTY(Replicated)
	bool bMovingForward;
	float RotationSpeed;
	float MovementSpeed;
	FVector EnterPosition; // When a character enters the ship, it will be transferred to this position. Not used yet.

	// Getters & Setters
	FRotator GetTargetRotation();

	/*UFUNCTION(Reliable, Server, WithValidation)
	void SetTargetRotation(FRotator newRot);
	virtual bool SetTargetRotation_Validate(FRotator newRot);
	virtual void SetTargetRotation_Implementation(FRotator newRot);*/

	UPROPERTY(Replicated)
	FRotator TargetRotation;

protected:

};
