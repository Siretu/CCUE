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

	void EnterShip(ACruiserCommandCharacter* character);

	// Variables
	UPROPERTY(Replicated)
	float RotationSpeed;
	float MovementSpeed;
	float CurrentSpeed;
	bool bBlocked;
	FVector EnterPosition; // When a character enters the ship, it will be transferred to this position. Not used yet.

	// Getters & Setters
	FRotator GetTargetRotation();

	/*UFUNCTION(Reliable, Server, WithValidation)
	void SetTargetRotation(FRotator newRot);
	virtual bool SetTargetRotation_Validate(FRotator newRot);
	virtual void SetTargetRotation_Implementation(FRotator newRot);*/

	UPROPERTY(Replicated)
	FRotator TargetRotation;

	UBoxComponent* ActivationBox;

	UFUNCTION()
	virtual void EnterConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:

};
