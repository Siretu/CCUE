// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GenericConsole.h"
#include "NavigationConsole.generated.h"

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API ANavigationConsole : public AGenericConsole
{
	GENERATED_BODY()
	
public:

	// Runs when a character leaves a console
	virtual void EnterConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	// Runs when a character leaves a console
	virtual void ExitConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	void ShipOrder();

	/** Sets the player's current ship's navigation. This should be in AShip, but RPCs don't seem to work when called from a different class. E.g: ship->SetTargetRotation doesn't work */
	UFUNCTION(Reliable, Server, WithValidation)
	void SetShipTargetRotation(AShip* s, FRotator newRot);
	virtual bool SetShipTargetRotation_Validate(AShip* s, FRotator newRot);
	virtual void SetShipTargetRotation_Implementation(AShip* s, FRotator newRot);
};
