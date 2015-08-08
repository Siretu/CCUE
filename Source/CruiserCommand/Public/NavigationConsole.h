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

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetShipTargetRotation(AShip* s, FRotator newRot);
	virtual bool ServerSetShipTargetRotation_Validate(AShip* s, FRotator newRot);
	virtual void ServerSetShipTargetRotation_Implementation(AShip* s, FRotator newRot);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerAccelerate();
	virtual bool ServerAccelerate_Validate();
	virtual void ServerAccelerate_Implementation();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerDecelerate();
	virtual bool ServerDecelerate_Validate();
	virtual void ServerDecelerate_Implementation();
};
